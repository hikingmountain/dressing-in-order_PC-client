#!/usr/bin/env python
# coding: utf-8

# # Demo of Dressing in Order
# In this notebook, we show how to use _Dressing in Order (DiOr)_ pipeline to run: (for 256x176)
# 1. pose transfer
# 2. try-on 
# 3. overlay
# 4. opacity change
# 5. content removal
# 6. content insertion
# 7. reshape
# 8. texture transfer
# 
# For more details, please check our paper and code at [DiOr webpage]().
# 
# NOTE: INDEX OF GARMENT is used as following:
# - 'top':5, # dress is also considered as top.
# - 'bottom':1,
# - 'hair':2,
# - 'jacket':3

import torch
import numpy as np
import cv2
import json
import os

from sympy import false

from models.dior_model import DIORModel
import utils.pose_utils as pose_utils
from datasets.deepfashion_datasets import DFVisualDataset

## this is a dummy "argparse"
class Opt:
    def __init__(self):
        pass


def create_opt(config):
    opt = Opt()
    opt.dataroot = config.get('dataroot')
    opt.isTrain = False
    opt.phase = 'test'
    opt.n_human_parts = 8;
    opt.n_kpts = 18;
    opt.style_nc = 64
    opt.n_style_blocks = 4;
    opt.netG = config.get('netG');
    opt.netE = 'adgan'
    opt.ngf = config.get('ngf')
    opt.norm_type = 'instance';
    opt.relu_type = 'leakyrelu'
    opt.init_type = 'orthogonal';
    opt.init_gain = 0.02;
    opt.gpu_ids = [0]
    opt.frozen_flownet = True;
    opt.random_rate = 1;
    opt.perturb = False;
    opt.warmup = False
    opt.name = config.get('exp_name')
    opt.vgg_path = config.get('vgg_path')
    opt.flownet_path = config.get('flownet_path')
    opt.checkpoints_dir = config.get('checkpoints_dir')
    opt.frozen_enc = True
    opt.load_iter = 0
    opt.epoch = config.get('epoch')
    opt.verbose = False
    return opt


def read_config(fn):
    config = json.load(open(fn))
    current_dir = os.path.dirname(os.path.abspath(__file__))
    data_root = os.path.join(current_dir, config.get('dataroot'))
    config['dataroot'] = data_root
    vgg_path = os.path.join(current_dir, config.get('vgg_path'))
    config['vgg_path'] = vgg_path
    flownet_path = os.path.join(current_dir, config.get('flownet_path'))
    config['flownet_path'] = flownet_path
    checkpoints_dir = os.path.join(current_dir, config.get('checkpoints_dir'))
    config['checkpoints_dir'] = checkpoints_dir
    return config


def load_pose_from_json(ani_pose_dir):
    with open(ani_pose_dir, 'r') as f:
        anno = json.load(f)
    len(anno['people'][0]['pose_keypoints_2d'])
    anno = list(anno['people'][0]['pose_keypoints_2d'])
    x = np.array(anno[1::3])
    y = np.array(anno[::3])

    coord = np.concatenate([x[:, None], y[:, None]], -1)
    # import pdb; pdb.set_trace()
    # coord = (coord * 1.1) - np.array([10,30])[None, :]
    pose = pose_utils.cords_to_map(coord, (256, 176), (256, 256))
    pose = np.transpose(pose, (2, 0, 1))
    pose = torch.Tensor(pose)
    return pose


def save_img(pimg=[], gimgs=[], gen_img=[]):
    # image = np.transpose(image.squeeze(0).numpy() * 255.0, (1, 2, 0)).astype(np.uint8)
    gimgs = torch.cat(gimgs, 2).float().cpu().detach().numpy()
    gimgs = (gimgs + 1) / 2  # denormalize
    gimgs = np.transpose(gimgs, [1, 2, 0])
    gimgs = (gimgs * 255.0).astype('uint8')
    gimgs = cv2.cvtColor(gimgs, cv2.COLOR_RGB2BGR)

    pimg = pimg.float().cpu().detach().numpy()
    pimg = (pimg + 1) / 2
    pimg = np.transpose(pimg, [1, 2, 0])
    pimg = (pimg * 255.0).astype('uint8')
    pimg = cv2.cvtColor(pimg, cv2.COLOR_RGB2BGR)

    gen_img = gen_img.float().cpu().detach().numpy()
    gen_img = (gen_img + 1) / 2
    gen_img = np.transpose(gen_img, [1, 2, 0])
    gen_img = (gen_img * 255.0).astype('uint8')
    gen_img = cv2.cvtColor(gen_img, cv2.COLOR_RGB2BGR)

    cv2.imwrite('gimg.jpg', gimgs)
    cv2.imwrite('pimg.jpg', pimg)
    cv2.imwrite('gen_img.jpg', gen_img)

    cv2.destroyAllWindows()

    # fig = plt.figure(figsize=(4, 2), dpi=100, facecolor='w', edgecolor='k')
    # plt.axis('off')
    # plt.imshow(gimgs)
    # plt.savefig('gimg.jpg')
    #
    # fig = plt.figure(figsize=(4, 4), dpi=100, facecolor='w', edgecolor='k')
    # plt.axis('off')
    # plt.imshow(gen_img)
    # plt.savefig('gen_img.jpg')
    #
    # plt.imshow(pimg)
    # plt.savefig('pimg.jpg')


def create_ds(config):
    Dataset = DFVisualDataset
    ds = Dataset(dataroot=config.get('dataroot'), dim=(256, 176), n_human_part=8)
    return ds


def create_model(config):
    opt = create_opt(config)
    # create model
    model = DIORModel(opt)
    model.setup(opt)
    return model


class TryOn:
    def __init__(self, fn):
        # current_dir = os.path.dirname(os.path.abspath(__file__))
        config = read_config(fn)
        self.ds = create_ds(config)
        self.model = create_model(config)


    # define some tool functions for I/O
    def load_img(self, pid, imgType):
        if isinstance(pid, str):  # load pose from scratch
            return None, None, load_pose_from_json(pid)

        # load model from scratch
        person = self.ds.get_inputs_by_key(pid[0], imgType)
        person = (i.cuda() for i in person)
        pimg, parse, to_pose = person
        return pimg.squeeze(), parse.squeeze(), to_pose.squeeze()


    # define dressing-in-order function (the pipeline)
    def dress_in_order(self, pid, pose_id=None, gids=[], ogids=[], order=[5, 1, 3, 2], perturb=False):
        PID = [0, 4, 6, 7]
        GID = [2, 5, 1, 3]
        # encode person
        pimg, parse, from_pose = self.load_img(pid, 'model')
        if perturb:
            pimg = self.model.perturb_images(pimg[None])[0]
        if not pose_id:
            to_pose = from_pose
        else:
            to_img, _, to_pose = self.load_img(pose_id)
        psegs = self.model.encode_attr(pimg[None], parse[None], from_pose[None], to_pose[None], PID)

        # encode base garments
        gsegs = self.model.encode_attr(pimg[None], parse[None], from_pose[None], to_pose[None])

        # swap base garment if any
        gimgs = []
        for gid in gids:
            _, _, k = gid
            gimg, gparse, pose = self.load_img(gid, 'garment')
            seg = self.model.encode_single_attr(gimg[None], gparse[None], pose[None], to_pose[None], i=gid[2])
            gsegs[gid[2]] = seg
            gimgs += [gimg * (gparse == gid[2])]

        # encode garment (overlay)
        garments = []
        over_gsegs = []
        oimgs = []
        for gid in ogids:
            oimg, oparse, pose = self.load_img(gid)
            oimgs += [oimg * (oparse == gid[2])]
            seg = self.model.encode_single_attr(oimg[None], oparse[None], pose[None], to_pose[None], i=gid[2])
            over_gsegs += [seg]

        gsegs = [gsegs[i] for i in order] + over_gsegs
        gen_img = self.model.netG(to_pose[None], psegs, gsegs)

        return pimg, gimgs, oimgs, gen_img[0], to_pose

    def generator(self, pid, gids, tuck):
        if tuck:
            # tuck in (dressing order: hair, top, bottom)
            pimg, gimgs, oimgs, gen_img, pose = self.dress_in_order(pid, gids=gids, order=[2, 5, 1])
        else:
            # not tuckin (dressing order: hair, bottom, top)
            pimg, gimgs, oimgs, gen_img, pose = self.dress_in_order(pid, gids=gids, order=[2, 1, 5])

        save_img(pimg, gimgs, gen_img=gen_img)


if __name__ == "__main__":
    tryon = TryOn('configForPython.json')

    # ## Layering - Multiple (dress in order)
    # person id
    p_id = ("fashionWOMENTees_Tanksid0000396708_1front.jpg", None, None)  # load person from the file
    # garments to try on (ordered)
    g_ids = [
        ("fashionWOMENTees_Tanksid0000361602_1front.jpg", None, 5),
        ("fashionWOMENGraphic_Teesid0000004401_1front.jpg", None, 1),
    ]

    # dressing in order
    tryon.generator(p_id, g_ids, False)




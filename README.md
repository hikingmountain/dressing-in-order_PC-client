## Introduction
While browsing the github website, I came across the code of the paper,["Dressing in Order: Recurrent Person Image Generation for Pose Transfer, Virtual Try-on and Outfit Editing"](https://openaccess.thecvf.com/content/ICCV2021/html/Cui_Dressing_in_Order_Recurrent_Person_Image_Generation_for_Pose_Transfer_ICCV_2021_paper.html) and found its functions very useful, especially the try-on function. <br/>
Nowadays, online shopping is very popular. When customers buy clothes online, they need a tool to try on the clothes they like. I think this dressing-in-order PC-client can meet their needs.

## Functions and Usage
This project consists of two modules. One is a graphical user interface module which was developed in C++ language based on Qt6 creator. This is my main contribution. The other is an image generation module, which is [the code](https://github.com/cuiaiyu/dressing-in-order) written by the original authors.
- Functions

The DNN model developed by the original author contains many functions, such as pose transfer, try on, texture transfer, reshaping, content removal, etc. Due to time constraints, the PC client only implemented the try-on function.

- Usage

Step1:  select a body image from Model Page.
<table><tr>
  <td><img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/step1_1.png" /></td>
  <td><img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/step1_2.jpg" /></td>
</tr></table>

Step2: select one top and one bottom from the images of Garment Page. Then press the \<tryon\> button at the bottom of the page.
<table><tr>
  <td><img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/step2_1.png" /></td>
  <td><img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/step2_2.png" /></td>
</tr></table>

Step3: You can tuck the top into the bottom by checking the \<Tuck\> checkbox at the bottom of the page.
<table><tr>
  <td><img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/step3_1.png" /></td>
  <td><img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/step3_2.png" /></td>
</tr></table>

## Enviroment setup
- C++ IDE
  
  Qt6.6.3
- Python Environment
  
  torch==2.3.0 <br/>
  torchvision==0.18.0 <br/>
  tensorboardX==2.6.2 <br/>
  scikit-image==0.24.0
- Change the three paths in configForCpp.json to your local path
  
  <img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/configForCpp.png" width="60%" />

- Copy configForCpp.json to the path in which the executable file is located  under \<build\> directory

- Change \<include\> path and \<lib\> path in CMakeLists.txt to your local path

  <img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/cmakelists.png" width="60%" />

- Change python library name in target_link_libraries to your python library name.

  <img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/cmakelists2.png" width="60%" />

- Download pretrained weight(DIOR_64.zip) from [google net-disk of the original authors](https://drive.google.com/drive/folders/1-7DxUvcrC3cvQV67Z2QhRdi-9PMDC8w9)

  Unzip DIOR_64.zip to dress-in-order/checkpoints. If you unzip it to different path, please modify the configForPython.json file under dress-in-order directory.
  
  <img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/configForPython_dior.png" width="60%" />

- Download vgg-19 model for pytorch from [here](https://download.pytorch.org/models/vgg19-dcbb9e9d.pth)
  Copy it to dress-in-order/checkpoints directory. If you copy it to different path, please modify the configForPython.json file under dress-in-order directory.

  <img src="https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/configForPython_vgg.png" width="60%" />


## Acknowledgements
If you find this work is helpful, please consider starring 🌟 this repository. <br/>
This repository is built up on [dress-in-order developed by the original authors](https://github.com/cuiaiyu/dressing-in-order). Thanks a lot for the great work to the authors.<br/>
Welcome to cite their paper!

## Citation

```
@InProceedings{Cui_2021_ICCV,
    author    = {Cui, Aiyu and McKee, Daniel and Lazebnik, Svetlana},
    title     = {Dressing in Order: Recurrent Person Image Generation for Pose Transfer, Virtual Try-On and Outfit Editing},
    booktitle = {Proceedings of the IEEE/CVF International Conference on Computer Vision (ICCV)},
    month     = {October},
    year      = {2021},
    pages     = {14638-14647}
}
```

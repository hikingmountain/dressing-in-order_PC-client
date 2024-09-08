## Introduction
While browsing the github website, I came across the code of the paper,["Dressing in Order: Recurrent Person Image Generation for Pose Transfer, Virtual Try-on and Outfit Editing"](https://openaccess.thecvf.com/content/ICCV2021/html/Cui_Dressing_in_Order_Recurrent_Person_Image_Generation_for_Pose_Transfer_ICCV_2021_paper.html) and found its functions very useful, especially the try-on function. Nowadays, online shopping is very popular. When customers are shopping clothes online, they need a tool to try on the clothes they like. I think this dressing-in-order PC-client can meet their needs.

## Functions and Usage
This project consists of two modules. One is a graphical user interface module which was developed in C++ language based on Qt6 creator, which is my main contribution. The other is an image generation module, which is [the code](https://github.com/cuiaiyu/dressing-in-order) written by the original authors.
- Functions
<br/>The DNN model developed by the original author contains many functions, such as pose transfer, try on, texture transfer, reshaping, content removal, etc. Due to time constraints, the PC client only implemented the try-on function.

- Usage
<br/>
step1 select a body image from Model Page.
![Image text](https://github.com/hikingmountain/dressing-in-order_PC-client/blob/main/readme_images/model_page1.png)
step2


3、软件使用步骤

## Enviroment setup
1、C++环境配置，配置文件的修改
2、python环境配置

## Acknowledgements
对论文作者表示感谢，号召用户引用他们的论文

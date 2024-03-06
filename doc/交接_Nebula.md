# 1.修改前端项目的后台域名
    .env.development 
    域名改成： 自定义
   
# 2.先进入打包前端项目
  yarn run build
  其他命令执行在package.json 文件scripts下

  、、、

  云端代码交接需要的材料：
  1.部署说明 
    小程序就是正常点击上传 去微信公众后台去发布
    运维平台执行命令 yarn build 会开始构建打包结果会在dist 文件夹下
  2.api文档
    前端跟后台api 文档一致
  3.功能简介
    nebula_cloud_portal ：Nebula运维平台
    nebula_miniapp : Nebula 小程序
  4.源码说明
    小程序使用原生 看微信公众平台开发文档就好
    运维平台使用 版本信息
    安装yarn 1.22.4  node 12.16.2 
    启动yarn serve
    编译yarn build
  5.账号密码、权限说明
    账号密码权限 同后台一致

  、、、

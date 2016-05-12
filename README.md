# HWRDemo

## Google input tools handwriting
> Google input tools使用的是https协议，需要libssl的以下三个库文件
- libssl32.dll
- libeay32.dll
- ssleay32.dll


## 汉王接口
（http://developer.hanvon.com/api/toAPIinfo.do?id=4&num=）
##### 请求接口

- 简体中文：http(s)://api.hanvon.com/rt/ws/v1/hand/line?key=yourKey&code=d4b92957-78ed-4c52-a004-ac3928b054b5
- 繁体中文：http(s)://api.hanvon.com/rt/ws/v1/hand/line?key=yourKey&code=05a7d172-ad21-4749-be0f-bfa4166d4da0
- 英文：http(s)://api.hanvon.com/rt/ws/v1/hand/line?key=yourKey&code=f01d64a2-bd96-4554-8bcc-81d221f314a4

key：请到开发中心-->应用管理-->Key管理，在您的Key列表中找到对应的Key
code：code为固定值，直接使用url中的code即可
请求参数描述     以JSON字符串形式，放到请求消息体中 
请求方法：POST
Content-Type：application/octet-stream

| 字段  | 描述  |
| :------------: | :------------: |
| uid |您的服务器IP地址。 |
|  lang | 中文简体：chns；中文繁体：chnt；英文：en  |
|  data |  手写行轨迹串. 格式是: x1,y1,x2,y2, -1,0, x3,y3,x4,y4,-1,0,-1,-1 |

每一笔写完以: -1,0结尾。
全部写完以：-1,-1结尾。
请求参数示例


```javascript
`{
    "uid": "118.12.0.12",
    "lang": "chns",
    "data": "x1,y1,x2,y2, -1,0, x3,y3,x4,y4,-1,0,-1,-1"
}`
```

返回值描述

字段	父字段	描述
code	response	0，表示成功。其它表示失败。
result	response	如果成功，则返回识别后的unicode码，多个码之间用”,”分隔。
返回值示例

Success:
```javascript
`{
    "code": "0",
    "result": "25105,123,105,0,25105,104,58,0,25105,104,63,0,25105,105,58,0,25105,123,58,0,25105,105,63,0,25105,123,63,0,"
}`
```
Failure:

```javascript
`{
    "code": "437",
    "result": "：input is null"
}`
```
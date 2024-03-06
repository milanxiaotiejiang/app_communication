# Nebula 后台服务API接口文档


**简介**:Nebula 后台服务API接口文档


**HOST**:172.30.128.1:8080


**联系人**:


**Version**:1.0


**接口路径**:/v2/api-docs


[TOC]






# OSS


## 删除


**接口地址**:`/nebula/sys/oss/file/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取查询签名


**接口地址**:`/nebula/sys/oss/file/getSign`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取查询签名</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|bucketName|bucketName|query|true|string||
|objectName|objectName|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 列表查询


**接口地址**:`/nebula/sys/oss/file/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|createBy|创建人|query|false|string||
|createTime|创建时间|query|false|string(date-time)||
|fileName||query|false|string||
|id|ID|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|updateBy|更新人|query|false|string||
|updateTime|更新时间|query|false|string(date-time)||
|url||query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«OSSFile»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«OSSFile»|IPage«OSSFile»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|OSSFile|
|&emsp;&emsp;&emsp;&emsp;createBy|创建人|string||
|&emsp;&emsp;&emsp;&emsp;createTime|创建时间|string||
|&emsp;&emsp;&emsp;&emsp;fileName||string||
|&emsp;&emsp;&emsp;&emsp;id|ID|string||
|&emsp;&emsp;&emsp;&emsp;updateBy|更新人|string||
|&emsp;&emsp;&emsp;&emsp;updateTime|更新时间|string||
|&emsp;&emsp;&emsp;&emsp;url||string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"createBy": "",
				"createTime": "",
				"fileName": "",
				"id": "",
				"updateBy": "",
				"updateTime": "",
				"url": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 通过id查询


**接口地址**:`/nebula/sys/oss/file/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>通过id查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«OSSFile»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|OSSFile|OSSFile|
|&emsp;&emsp;createBy|创建人|string||
|&emsp;&emsp;createTime|创建时间|string(date-time)||
|&emsp;&emsp;fileName||string||
|&emsp;&emsp;id|ID|string||
|&emsp;&emsp;updateBy|更新人|string||
|&emsp;&emsp;updateTime|更新时间|string(date-time)||
|&emsp;&emsp;url||string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"createBy": "",
		"createTime": "",
		"fileName": "",
		"id": "",
		"updateBy": "",
		"updateTime": "",
		"url": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 生成直传签名


**接口地址**:`/nebula/sys/oss/file/sign`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>生成直传签名</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|bucketType|文件类型1意见类型2手册图片3手册视频4培训资料|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 上传


**接口地址**:`/nebula/sys/oss/file/upload`


**请求方式**:`POST`


**请求数据类型**:`multipart/form-data`


**响应数据类型**:`*/*`


**接口描述**:<p>上传</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|file|file|body|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# OTA


## 生成升级包文件URL


**接口地址**:`/nebula/ota/GenerateOtaUploadUri`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>生成升级包文件URL</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|fileSuffix|升级包名后缀|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 重升级


**接口地址**:`/nebula/ota/ReupgradeOTATask`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>重升级</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包Id|query|true|string||
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 取消设备升级


**接口地址**:`/nebula/ota/cancel/device/upgrade`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>取消设备升级</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|firmwareId|升级包Id|query|true|string||
|jobId|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 取消批次升级


**接口地址**:`/nebula/ota/cancel/task/upgrade`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>取消批次升级</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包id|query|true|string||
|jobId|批次Id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 创建升级包


**接口地址**:`/nebula/ota/createOTAFirmware`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>创建升级包</p>



**请求示例**:


```javascript
{
  "destVersion": "",
  "firmwareDesc": "",
  "firmwareName": "",
  "firmwareSign": "",
  "firmwareSize": 0,
  "firmwareUrl": "",
  "iotInstanceId": "",
  "moduleName": "",
  "needToVerify": true,
  "otaFirmwareFileDtoList": [
    {
      "destVersion": "",
      "firmwareSign": "",
      "firmwareSize": 0,
      "firmwareUrl": "",
      "moduleName": ""
    }
  ],
  "productKey": "",
  "productName": "",
  "signMethod": "",
  "srcVersion": "",
  "type": 0,
  "udi": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|otaFirmwareDto|otaFirmwareDto|body|true|OtaFirmwareDto|OtaFirmwareDto|
|&emsp;&emsp;destVersion|当前OTA升级包的版本号||true|string||
|&emsp;&emsp;firmwareDesc|OTA升级包描述||false|string||
|&emsp;&emsp;firmwareName|OTA升级包名称||true|string||
|&emsp;&emsp;firmwareSign|OTA升级包文件的签名值||false|string||
|&emsp;&emsp;firmwareSize|OTA升级包文件的大小||false|integer(int32)||
|&emsp;&emsp;firmwareUrl|OTA升级包文件的URL||false|string||
|&emsp;&emsp;iotInstanceId|||false|string||
|&emsp;&emsp;moduleName|OTA模块名称||false|string||
|&emsp;&emsp;needToVerify|是否需要在创建批量升级任务前通过升级包验证||false|boolean||
|&emsp;&emsp;otaFirmwareFileDtoList|升级包信息||false|array|OtaFirmwareFileDto|
|&emsp;&emsp;&emsp;&emsp;destVersion|待升级OTA模块版本号||false|string||
|&emsp;&emsp;&emsp;&emsp;firmwareSign|OTA升级包文件的签名值||false|string||
|&emsp;&emsp;&emsp;&emsp;firmwareSize|OTA升级包文件的大小||false|integer||
|&emsp;&emsp;&emsp;&emsp;firmwareUrl|OTA升级包文件的URL||false|string||
|&emsp;&emsp;&emsp;&emsp;moduleName|OTA模块名称||false|string||
|&emsp;&emsp;productKey|TA升级包所属产品的ProductKey||true|string||
|&emsp;&emsp;productName|产品名称||true|string||
|&emsp;&emsp;signMethod|OTA升级包签名方法||false|string||
|&emsp;&emsp;srcVersion|待升级OTA模块版本号||false|string||
|&emsp;&emsp;type|OTA升级包类型||false|integer(int32)||
|&emsp;&emsp;udi|推送给设备的自定义信息||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## ota静态升级 立即升级


**接口地址**:`/nebula/ota/createOtaStaticUpgradeJob`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>ota静态升级 立即升级</p>



**请求示例**:


```javascript
{
  "dnListFileUrl": "",
  "downloadProtocol": "",
  "endTime": "",
  "firmwareId": "",
  "firmwareName": "",
  "grayPercent": "",
  "groupId": "",
  "groupType": "",
  "iotInstanceId": "",
  "maximumPerMinute": 0,
  "multiModuleMode": true,
  "needConfirm": true,
  "needPush": true,
  "overwriteMode": 0,
  "productKey": "",
  "productName": "",
  "retryCount": 0,
  "retryInterval": 0,
  "scheduleFinishTime": 0,
  "scheduleTime": 0,
  "srcVersion": [],
  "startTime": "",
  "tag": [
    {
      "key": "",
      "value": ""
    }
  ],
  "targetDeviceName": [],
  "targetSelection": "",
  "timeoutInMinutes": 0,
  "upgradeType": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|staticUpgradeDto|staticUpgradeDto|body|true|StaticUpgradeDto|StaticUpgradeDto|
|&emsp;&emsp;dnListFileUrl|定向升级设备列表文件的URL||false|string||
|&emsp;&emsp;downloadProtocol|升级包下载协议||false|string||
|&emsp;&emsp;endTime|升级结束时间||true|string||
|&emsp;&emsp;firmwareId|升级包ID||true|string||
|&emsp;&emsp;firmwareName|升级包名字||false|string||
|&emsp;&emsp;grayPercent|设置灰度比例||false|string||
|&emsp;&emsp;groupId|分组类型id||false|string||
|&emsp;&emsp;groupType|分组类型||false|string||
|&emsp;&emsp;iotInstanceId|实例ID||false|string||
|&emsp;&emsp;maximumPerMinute|每分钟最多向多少个设备推送升级包||false|integer(int32)||
|&emsp;&emsp;multiModuleMode|设备是否支持多模块同时升级||false|boolean||
|&emsp;&emsp;needConfirm|是否需要app确认||false|boolean||
|&emsp;&emsp;needPush|物联网平台是否主动向设备推送升级任务||false|boolean||
|&emsp;&emsp;overwriteMode|是否覆盖之前的升级任务||false|integer(int32)||
|&emsp;&emsp;productKey|升级包所属产品||false|string||
|&emsp;&emsp;productName|产品名称||false|string||
|&emsp;&emsp;retryCount|自动重试次数||false|integer(int32)||
|&emsp;&emsp;retryInterval|失败重试时间间隔||false|integer(int32)||
|&emsp;&emsp;scheduleFinishTime|指定结束升级的时间||false|integer(int64)||
|&emsp;&emsp;scheduleTime|指定发起OTA升级的时间||false|integer(int64)||
|&emsp;&emsp;srcVersion|待升级版本号列表||false|array|string|
|&emsp;&emsp;startTime|升级开始时间||true|string||
|&emsp;&emsp;tag|标签列表||false|array|CreateOTAStaticUpgradeJobRequestTag|
|&emsp;&emsp;&emsp;&emsp;key|||false|string||
|&emsp;&emsp;&emsp;&emsp;value|||false|string||
|&emsp;&emsp;targetDeviceName|定向升级的设备名称列表||true|array|string|
|&emsp;&emsp;targetSelection|升级范围  ALL：全量升级。SPECIFIC：定向升级。GRAY：灰度升级。GROUP：分组升级||true|string||
|&emsp;&emsp;timeoutInMinutes|设备升级超时时间||false|integer(int32)||
|&emsp;&emsp;upgradeType|升级类型||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除升级包


**接口地址**:`/nebula/ota/delFirmware`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除升级包</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|firmwareId|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 升级包列表


**接口地址**:`/nebula/ota/firmwarePage`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>升级包列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|firmwareName|升级包名称|query|false|string||
|module|模块|query|false|string||
|product|产品|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«OtaFirmwarePageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«OtaFirmwarePageVo»|IPage«OtaFirmwarePageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|OtaFirmwarePageVo|
|&emsp;&emsp;&emsp;&emsp;createTime|创建时间|integer||
|&emsp;&emsp;&emsp;&emsp;destVersion|当前OTA升级包的版本号|string||
|&emsp;&emsp;&emsp;&emsp;firmwareId|升级包id|string||
|&emsp;&emsp;&emsp;&emsp;firmwareName|OTA升级包名称|string||
|&emsp;&emsp;&emsp;&emsp;moduleName|OTA模块名称|string||
|&emsp;&emsp;&emsp;&emsp;productKey|TA升级包所属产品id|string||
|&emsp;&emsp;&emsp;&emsp;productName|TA升级包所属产品|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"createTime": 0,
				"destVersion": "",
				"firmwareId": "",
				"firmwareName": "",
				"moduleName": "",
				"productKey": "",
				"productName": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 获取升级包包含的版本


**接口地址**:`/nebula/ota/getVersions`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取升级包包含的版本</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包Id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取批次状态


**接口地址**:`/nebula/ota/job/status`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取批次状态</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|jobId|批次id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 升级包信息


**接口地址**:`/nebula/ota/otaFirmwareInfo`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>升级包信息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«OtaFirmwareInfoVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|OtaFirmwareInfoVo|OtaFirmwareInfoVo|
|&emsp;&emsp;coreVersion|Core版本|string||
|&emsp;&emsp;createTime|创建时间|integer(int64)||
|&emsp;&emsp;destVersion|升级包版本|string||
|&emsp;&emsp;ecuVersion|Ecu版本|string||
|&emsp;&emsp;firmwareDesc|升级包描述|string||
|&emsp;&emsp;firmwareId|升级包ID|string||
|&emsp;&emsp;firmwareName|升级包名称|string||
|&emsp;&emsp;moduleName|升级包类型|string||
|&emsp;&emsp;nebulaVersion|Nebula版本|string||
|&emsp;&emsp;padVersion|Pad版本|string||
|&emsp;&emsp;productKey|产品key|string||
|&emsp;&emsp;productName|升级产品|string||
|&emsp;&emsp;type|升级包类型|integer(int32)||
|&emsp;&emsp;udi|推送给设备的自定义信息|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"coreVersion": "",
		"createTime": 0,
		"destVersion": "",
		"ecuVersion": "",
		"firmwareDesc": "",
		"firmwareId": "",
		"firmwareName": "",
		"moduleName": "",
		"nebulaVersion": "",
		"padVersion": "",
		"productKey": "",
		"productName": "",
		"type": 0,
		"udi": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 根据产品获取模块


**接口地址**:`/nebula/ota/otaModuleByProduct`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据产品获取模块</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|product|产品|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 升级批次设备详情列表


**接口地址**:`/nebula/ota/upgradeDeviceJobDetailPage`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>升级批次设备详情列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包id|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|versionType|版本类型|query|true|string||
|deviceName|设备名|query|false|string||
|jobId|批次id|query|false|string||
|status|状态|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«OtaDeviceUpgreadeTaskPageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«OtaDeviceUpgreadeTaskPageVo»|IPage«OtaDeviceUpgreadeTaskPageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|OtaDeviceUpgreadeTaskPageVo|
|&emsp;&emsp;&emsp;&emsp;deviceName|设备编号|string||
|&emsp;&emsp;&emsp;&emsp;deviceRosVersion|ros版本|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftTaskDesc|ecu任务详情|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftTaskStatus|ecu升级状态|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftUpdateTime|ecu更新时间|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftVersion|ecu版本|string||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;jobId|批次id|string||
|&emsp;&emsp;&emsp;&emsp;nebulaTaskDesc|nebula任务详情|string||
|&emsp;&emsp;&emsp;&emsp;nebulaTaskStatus|nebula升级状态|string||
|&emsp;&emsp;&emsp;&emsp;nebulaUpdateTime|nebula更新时间|string||
|&emsp;&emsp;&emsp;&emsp;nebulaVersion|nebula版本|string||
|&emsp;&emsp;&emsp;&emsp;nickname|设备名|string||
|&emsp;&emsp;&emsp;&emsp;padTaskDesc|pad任务详情|string||
|&emsp;&emsp;&emsp;&emsp;padTaskStatus|pad升级状态|string||
|&emsp;&emsp;&emsp;&emsp;padUpdateTime|pad更新时间|string||
|&emsp;&emsp;&emsp;&emsp;padVersion|pad版本|string||
|&emsp;&emsp;&emsp;&emsp;productName|设备所属产品|string||
|&emsp;&emsp;&emsp;&emsp;rosTaskDesc|ros任务详情|string||
|&emsp;&emsp;&emsp;&emsp;rosTaskStatus|ros升级状态|string||
|&emsp;&emsp;&emsp;&emsp;rosUpdateTime|ros更新时间|string||
|&emsp;&emsp;&emsp;&emsp;taskStatus|状态|string||
|&emsp;&emsp;&emsp;&emsp;utcModified|状态更新时间|string||
|&emsp;&emsp;&emsp;&emsp;version|当前版本号|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"deviceName": "",
				"deviceRosVersion": "",
				"ecuSoftTaskDesc": "",
				"ecuSoftTaskStatus": "",
				"ecuSoftUpdateTime": "",
				"ecuSoftVersion": "",
				"id": "",
				"jobId": "",
				"nebulaTaskDesc": "",
				"nebulaTaskStatus": "",
				"nebulaUpdateTime": "",
				"nebulaVersion": "",
				"nickname": "",
				"padTaskDesc": "",
				"padTaskStatus": "",
				"padUpdateTime": "",
				"padVersion": "",
				"productName": "",
				"rosTaskDesc": "",
				"rosTaskStatus": "",
				"rosUpdateTime": "",
				"taskStatus": "",
				"utcModified": "",
				"version": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 升级批次设备列表


**接口地址**:`/nebula/ota/upgradeDeviceJobPage`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>升级批次设备列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包id|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|versionType|版本类型|query|true|string||
|deviceName|设备名|query|false|string||
|jobId|批次id|query|false|string||
|status|状态|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«OtaDeviceUpgreadeTaskPageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«OtaDeviceUpgreadeTaskPageVo»|IPage«OtaDeviceUpgreadeTaskPageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|OtaDeviceUpgreadeTaskPageVo|
|&emsp;&emsp;&emsp;&emsp;deviceName|设备编号|string||
|&emsp;&emsp;&emsp;&emsp;deviceRosVersion|ros版本|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftTaskDesc|ecu任务详情|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftTaskStatus|ecu升级状态|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftUpdateTime|ecu更新时间|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftVersion|ecu版本|string||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;jobId|批次id|string||
|&emsp;&emsp;&emsp;&emsp;nebulaTaskDesc|nebula任务详情|string||
|&emsp;&emsp;&emsp;&emsp;nebulaTaskStatus|nebula升级状态|string||
|&emsp;&emsp;&emsp;&emsp;nebulaUpdateTime|nebula更新时间|string||
|&emsp;&emsp;&emsp;&emsp;nebulaVersion|nebula版本|string||
|&emsp;&emsp;&emsp;&emsp;nickname|设备名|string||
|&emsp;&emsp;&emsp;&emsp;padTaskDesc|pad任务详情|string||
|&emsp;&emsp;&emsp;&emsp;padTaskStatus|pad升级状态|string||
|&emsp;&emsp;&emsp;&emsp;padUpdateTime|pad更新时间|string||
|&emsp;&emsp;&emsp;&emsp;padVersion|pad版本|string||
|&emsp;&emsp;&emsp;&emsp;productName|设备所属产品|string||
|&emsp;&emsp;&emsp;&emsp;rosTaskDesc|ros任务详情|string||
|&emsp;&emsp;&emsp;&emsp;rosTaskStatus|ros升级状态|string||
|&emsp;&emsp;&emsp;&emsp;rosUpdateTime|ros更新时间|string||
|&emsp;&emsp;&emsp;&emsp;taskStatus|状态|string||
|&emsp;&emsp;&emsp;&emsp;utcModified|状态更新时间|string||
|&emsp;&emsp;&emsp;&emsp;version|当前版本号|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"deviceName": "",
				"deviceRosVersion": "",
				"ecuSoftTaskDesc": "",
				"ecuSoftTaskStatus": "",
				"ecuSoftUpdateTime": "",
				"ecuSoftVersion": "",
				"id": "",
				"jobId": "",
				"nebulaTaskDesc": "",
				"nebulaTaskStatus": "",
				"nebulaUpdateTime": "",
				"nebulaVersion": "",
				"nickname": "",
				"padTaskDesc": "",
				"padTaskStatus": "",
				"padUpdateTime": "",
				"padVersion": "",
				"productName": "",
				"rosTaskDesc": "",
				"rosTaskStatus": "",
				"rosUpdateTime": "",
				"taskStatus": "",
				"utcModified": "",
				"version": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 升级批次列表


**接口地址**:`/nebula/ota/upgradeJobPage`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>升级批次列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包id|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|jobId|批次id|query|false|string||
|jobStatus|升级包id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«OtaUpgradeJobPageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«OtaUpgradeJobPageVo»|IPage«OtaUpgradeJobPageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|OtaUpgradeJobPageVo|
|&emsp;&emsp;&emsp;&emsp;coreVersion|Core版本|string||
|&emsp;&emsp;&emsp;&emsp;ecuVersion|Ecu版本|string||
|&emsp;&emsp;&emsp;&emsp;firmwareId|升级包id|string||
|&emsp;&emsp;&emsp;&emsp;jobId|批次id|string||
|&emsp;&emsp;&emsp;&emsp;jobStatus|状态|string||
|&emsp;&emsp;&emsp;&emsp;jobType|批次类型|string||
|&emsp;&emsp;&emsp;&emsp;nebulaVersion|Nebula版本|string||
|&emsp;&emsp;&emsp;&emsp;padVersion|Pad版本|string||
|&emsp;&emsp;&emsp;&emsp;upgradeType|升级策略|string||
|&emsp;&emsp;&emsp;&emsp;utcCreate|创建时间|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"coreVersion": "",
				"ecuVersion": "",
				"firmwareId": "",
				"jobId": "",
				"jobStatus": "",
				"jobType": "",
				"nebulaVersion": "",
				"padVersion": "",
				"upgradeType": "",
				"utcCreate": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 批次信息


**接口地址**:`/nebula/ota/upgradeTaskJobInfo`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>批次信息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包id|query|true|string||
|jobId|批次id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«UpgradeTaskJobInfoVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|UpgradeTaskJobInfoVo|UpgradeTaskJobInfoVo|
|&emsp;&emsp;coreVersion|Core版本|string||
|&emsp;&emsp;destVersion|升级包版本号|string||
|&emsp;&emsp;ecuVersion|Ecu版本|string||
|&emsp;&emsp;jobId|批次id|string||
|&emsp;&emsp;moduleName|升级模块|string||
|&emsp;&emsp;nebulaVersion|Nebula版本|string||
|&emsp;&emsp;padVersion|Pad版本|string||
|&emsp;&emsp;productName|所属产品|string||
|&emsp;&emsp;targetSelection|升级范围|string||
|&emsp;&emsp;timeoutInMinutes|设备升级超时时间|integer(int32)||
|&emsp;&emsp;upgradeType|升级策略|string||
|&emsp;&emsp;utcStartTime|升级时间|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"coreVersion": "",
		"destVersion": "",
		"ecuVersion": "",
		"jobId": "",
		"moduleName": "",
		"nebulaVersion": "",
		"padVersion": "",
		"productName": "",
		"targetSelection": "",
		"timeoutInMinutes": 0,
		"upgradeType": "",
		"utcStartTime": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 查询升级包成功失败数


**接口地址**:`/nebula/ota/upgradeTaskJobStatistics`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询升级包成功失败数</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«UpgradeTaskJobStatisticsVO»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|UpgradeTaskJobStatisticsVO|UpgradeTaskJobStatisticsVO|
|&emsp;&emsp;deviceAccess|目标成功数|integer(int32)||
|&emsp;&emsp;deviceCancel|目标取消数|integer(int32)||
|&emsp;&emsp;deviceCount|目标设备总数|integer(int32)||
|&emsp;&emsp;deviceFail|目标失败数|integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"deviceAccess": 0,
		"deviceCancel": 0,
		"deviceCount": 0,
		"deviceFail": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 查询升级包成功失败数详情数


**接口地址**:`/nebula/ota/upgradeTaskJobStatisticsDetail`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询升级包成功失败数详情数</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|jobId|批次id|query|true|string||
|type|升级类型|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«DeviceUpgradeTaskJobStatisticsVO»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|DeviceUpgradeTaskJobStatisticsVO|DeviceUpgradeTaskJobStatisticsVO|
|&emsp;&emsp;allStatus|所有状态|integer(int32)||
|&emsp;&emsp;canceledStatus|已取消|integer(int32)||
|&emsp;&emsp;confirmStatus|待确认|integer(int32)||
|&emsp;&emsp;failedStatus|升级失败|integer(int32)||
|&emsp;&emsp;inProgressStatus|升级中|integer(int32)||
|&emsp;&emsp;notifiedStatus|已推送|integer(int32)||
|&emsp;&emsp;queuedStatus|待推送|integer(int32)||
|&emsp;&emsp;succeededStatus|升级成功|integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"allStatus": 0,
		"canceledStatus": 0,
		"confirmStatus": 0,
		"failedStatus": 0,
		"inProgressStatus": 0,
		"notifiedStatus": 0,
		"queuedStatus": 0,
		"succeededStatus": 0
	},
	"success": true,
	"timestamp": 0
}
```


# amqp-controller


## 测试分布式锁


**接口地址**:`/nebula/amqp/lock`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>测试分布式锁</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 查看线程队列情况


**接口地址**:`/nebula/amqp/query/thread/info`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查看线程队列情况</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«ThreadInfoVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array|ThreadInfoVo|
|&emsp;&emsp;activeCount1|当前活动线程数|integer(int32)||
|&emsp;&emsp;activeCount2|当前活动线程数|integer(int32)||
|&emsp;&emsp;completedTaskCount1|执行完成线程数|integer(int64)||
|&emsp;&emsp;completedTaskCount2|执行完成线程数|integer(int64)||
|&emsp;&emsp;connectionStatus|连接状态|boolean||
|&emsp;&emsp;key|key|string||
|&emsp;&emsp;localAddr|客户端ip|string||
|&emsp;&emsp;queueSize1|当前排队线程数|integer(int32)||
|&emsp;&emsp;queueSize2|当前排队线程数|integer(int32)||
|&emsp;&emsp;serverPort|客户端端口|string||
|&emsp;&emsp;taskCount1|总线程数|integer(int64)||
|&emsp;&emsp;taskCount2|总线程数|integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [
		{
			"activeCount1": 0,
			"activeCount2": 0,
			"completedTaskCount1": 0,
			"completedTaskCount2": 0,
			"connectionStatus": true,
			"key": "",
			"localAddr": "",
			"queueSize1": 0,
			"queueSize2": 0,
			"serverPort": "",
			"taskCount1": 0,
			"taskCount2": 0
		}
	],
	"success": true,
	"timestamp": 0
}
```


## 删除


**接口地址**:`/nebula/amqp/start/monitor/delKey`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|key|key|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 开始监听topic


**接口地址**:`/nebula/amqp/start/monitor/topic`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>开始监听topic</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 停止监听topic


**接口地址**:`/nebula/amqp/stop/monitor/topic`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>停止监听topic</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# pad接口


## 获取跳转清洁记录二维码


**接口地址**:`/nebula/pad/cleantask/path/file`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取跳转清洁记录二维码</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 获取设备版本


**接口地址**:`/nebula/pad/version`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备版本</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|coreVersion|core版本|query|true|string||
|ecuHardVersion|core版本|query|true|string||
|ecuSoftVersion|core版本|query|true|string||
|padVersion|core版本|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«DeviceVersionVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|DeviceVersionVo|DeviceVersionVo|
|&emsp;&emsp;deviceRosVersion|ros版本|VersionVo|VersionVo|
|&emsp;&emsp;&emsp;&emsp;sprint||string||
|&emsp;&emsp;&emsp;&emsp;version||string||
|&emsp;&emsp;ecuHardVersion|ecu硬件版本|VersionVo|VersionVo|
|&emsp;&emsp;&emsp;&emsp;sprint||string||
|&emsp;&emsp;&emsp;&emsp;version||string||
|&emsp;&emsp;ecuSoftVersion|ecu软件版本|VersionVo|VersionVo|
|&emsp;&emsp;&emsp;&emsp;sprint||string||
|&emsp;&emsp;&emsp;&emsp;version||string||
|&emsp;&emsp;padVersion|pad版本|VersionVo|VersionVo|
|&emsp;&emsp;&emsp;&emsp;sprint||string||
|&emsp;&emsp;&emsp;&emsp;version||string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"deviceRosVersion": {
			"sprint": "",
			"version": ""
		},
		"ecuHardVersion": {
			"sprint": "",
			"version": ""
		},
		"ecuSoftVersion": {
			"sprint": "",
			"version": ""
		},
		"padVersion": {
			"sprint": "",
			"version": ""
		}
	},
	"success": true,
	"timestamp": 0
}
```


# proxy-controller


## 获取proxy地址


**接口地址**:`/nebula/proxy/point`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取proxy地址</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# robot-controller


## 获取所有设备数量


**接口地址**:`/nebula/robot/all/num`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取所有设备数量</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 分页列表查询


**接口地址**:`/nebula/robot/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«RobotInfoVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«RobotInfoVo»|IPage«RobotInfoVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|RobotInfoVo|
|&emsp;&emsp;&emsp;&emsp;date_time||string||
|&emsp;&emsp;&emsp;&emsp;device_name||string||
|&emsp;&emsp;&emsp;&emsp;device_project||string||
|&emsp;&emsp;&emsp;&emsp;device_ros_version||string||
|&emsp;&emsp;&emsp;&emsp;drag_status||string||
|&emsp;&emsp;&emsp;&emsp;dust_box_status||string||
|&emsp;&emsp;&emsp;&emsp;electric||string||
|&emsp;&emsp;&emsp;&emsp;geo_name||string||
|&emsp;&emsp;&emsp;&emsp;iot_id||string||
|&emsp;&emsp;&emsp;&emsp;is_urgency_stop||string||
|&emsp;&emsp;&emsp;&emsp;lock_status||string||
|&emsp;&emsp;&emsp;&emsp;map_name||string||
|&emsp;&emsp;&emsp;&emsp;push_status||string||
|&emsp;&emsp;&emsp;&emsp;sweep_status||string||
|&emsp;&emsp;&emsp;&emsp;timestamp||string||
|&emsp;&emsp;&emsp;&emsp;water_box_status||string||
|&emsp;&emsp;&emsp;&emsp;work_status_code||string||
|&emsp;&emsp;&emsp;&emsp;work_status_message||string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"date_time": "",
				"device_name": "",
				"device_project": "",
				"device_ros_version": "",
				"drag_status": "",
				"dust_box_status": "",
				"electric": "",
				"geo_name": "",
				"iot_id": "",
				"is_urgency_stop": "",
				"lock_status": "",
				"map_name": "",
				"push_status": "",
				"sweep_status": "",
				"timestamp": "",
				"water_box_status": "",
				"work_status_code": "",
				"work_status_message": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 获取在线设备数量


**接口地址**:`/nebula/robot/online/num`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取在线设备数量</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 产品手册配置


## 获取所在分类


**接口地址**:`/nebula/manual/category`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取所在分类</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|type|产品介绍：1  产品使用手册视频：2|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 新增产品介绍或视频手册


**接口地址**:`/nebula/manual/create`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>新增产品介绍或视频手册</p>



**请求示例**:


```javascript
{
  "category": "",
  "content": "",
  "id": "",
  "imageFileName": "",
  "imageFileUrl": "",
  "sort": 0,
  "title": "",
  "type": 0,
  "videoFileName": "",
  "videoFileUrl": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|manualVo|manualVo|body|true|ManualVo|ManualVo|
|&emsp;&emsp;category|||false|string||
|&emsp;&emsp;content|||false|string||
|&emsp;&emsp;id|||false|string||
|&emsp;&emsp;imageFileName|||false|string||
|&emsp;&emsp;imageFileUrl|||false|string||
|&emsp;&emsp;sort|||false|integer(int32)||
|&emsp;&emsp;title|||false|string||
|&emsp;&emsp;type|||false|integer(int32)||
|&emsp;&emsp;videoFileName|||false|string||
|&emsp;&emsp;videoFileUrl|||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除


**接口地址**:`/nebula/manual/deleted/{id}`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|主键|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 判断是否存在发布产品介绍视频


**接口地址**:`/nebula/manual/exists/manual/video`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>判断是否存在发布产品介绍视频</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取基本信息接口


**接口地址**:`/nebula/manual/info`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取基本信息接口</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|主键|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取产品介绍url


**接口地址**:`/nebula/manual/manual/video`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取产品介绍url</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 分页列表查询


**接口地址**:`/nebula/manual/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|category|所在分类|query|false|string||
|sort|排序|query|false|string||
|status|状态|query|false|boolean||
|title|标题|query|false|string||
|type|视频类型|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«ManualPageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«ManualPageVo»|IPage«ManualPageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|ManualPageVo|
|&emsp;&emsp;&emsp;&emsp;category||string||
|&emsp;&emsp;&emsp;&emsp;hasContent||boolean||
|&emsp;&emsp;&emsp;&emsp;id||string||
|&emsp;&emsp;&emsp;&emsp;imageFileName||string||
|&emsp;&emsp;&emsp;&emsp;imageFileUrl||string||
|&emsp;&emsp;&emsp;&emsp;status||boolean||
|&emsp;&emsp;&emsp;&emsp;title||string||
|&emsp;&emsp;&emsp;&emsp;type||integer||
|&emsp;&emsp;&emsp;&emsp;videoFileName||string||
|&emsp;&emsp;&emsp;&emsp;videoFileUrl||string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"category": "",
				"hasContent": true,
				"id": "",
				"imageFileName": "",
				"imageFileUrl": "",
				"status": true,
				"title": "",
				"type": 0,
				"videoFileName": "",
				"videoFileUrl": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 发布或取消


**接口地址**:`/nebula/manual/publish/{status}`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>发布或取消</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|主键|query|true|string||
|status|0取消1发版|query|true|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 更新


**接口地址**:`/nebula/manual/update`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>更新</p>



**请求示例**:


```javascript
{
  "category": "",
  "content": "",
  "id": "",
  "imageFileName": "",
  "imageFileUrl": "",
  "sort": 0,
  "title": "",
  "type": 0,
  "videoFileName": "",
  "videoFileUrl": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|manualVo|manualVo|body|true|ManualVo|ManualVo|
|&emsp;&emsp;category|||false|string||
|&emsp;&emsp;content|||false|string||
|&emsp;&emsp;id|||false|string||
|&emsp;&emsp;imageFileName|||false|string||
|&emsp;&emsp;imageFileUrl|||false|string||
|&emsp;&emsp;sort|||false|integer(int32)||
|&emsp;&emsp;title|||false|string||
|&emsp;&emsp;type|||false|integer(int32)||
|&emsp;&emsp;videoFileName|||false|string||
|&emsp;&emsp;videoFileUrl|||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 详解预览


**接口地址**:`/nebula/manual/view`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>详解预览</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|主键|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 任务管理


## 创建定时任务


**接口地址**:`/nebula/task/create`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>创建定时任务</p>



**请求示例**:


```javascript
{
  "deviceName": "",
  "endDay": 0,
  "endMonth": 0,
  "endYear": 0,
  "id": "",
  "iotId": "",
  "isExecute": 0,
  "isNever": 0,
  "isSkip": 0,
  "name": "",
  "omapId": "",
  "rate": 0,
  "taskId": 0,
  "taskName": "",
  "timerId": "",
  "timerRule": "",
  "type": 0
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|taskDto|taskDto|body|true|TaskDto|TaskDto|
|&emsp;&emsp;deviceName|设备名||false|string||
|&emsp;&emsp;endDay|日||false|integer(int32)||
|&emsp;&emsp;endMonth|月||false|integer(int32)||
|&emsp;&emsp;endYear|年||false|integer(int32)||
|&emsp;&emsp;id|主键||false|string||
|&emsp;&emsp;iotId|设备id||false|string||
|&emsp;&emsp;isExecute|是否开启||false|integer(int32)||
|&emsp;&emsp;isNever|截止日期||false|integer(int32)||
|&emsp;&emsp;isSkip|跳过节假日||false|integer(int32)||
|&emsp;&emsp;name|任务名称||false|string||
|&emsp;&emsp;omapId|||false|string||
|&emsp;&emsp;rate|执行频率||false|integer(int32)||
|&emsp;&emsp;taskId|组合任务id||false|integer(int64)||
|&emsp;&emsp;taskName|组合任务name||false|string||
|&emsp;&emsp;timerId|定时任务id||false|string||
|&emsp;&emsp;timerRule|规则||false|string||
|&emsp;&emsp;type|类型||false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 创建普通任务


**接口地址**:`/nebula/task/create/combine`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>创建普通任务</p>



**请求示例**:


```javascript
{
  "aromatherapyType": 0,
  "cleanType": 0,
  "id": "",
  "iotId": "",
  "mode": 0,
  "name": "",
  "omapId": "",
  "rate": 0,
  "zones": [
    {
      "points": [
        {
          "x": 0,
          "y": 0
        }
      ],
      "zoneId": 0
    }
  ]
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|createCombineDto|createCombineDto|body|true|CreateCombineDto|CreateCombineDto|
|&emsp;&emsp;aromatherapyType|香薰/消杀 0关闭，1消杀，3香薰||false|integer(int32)||
|&emsp;&emsp;cleanType|清洁模式 0关闭，1清扫，2湿拖，3尘吸，4尘推，5扫吸和尘推||false|integer(int32)||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;iotId|iotId||false|string||
|&emsp;&emsp;mode|0 Zoned 划区,1 Cover 全覆盖,2 Subregion 分区,3 Line 沿边||false|integer(int32)||
|&emsp;&emsp;name|任务名称||false|string||
|&emsp;&emsp;omapId|||false|string||
|&emsp;&emsp;rate|次数||false|integer(int32)||
|&emsp;&emsp;zones|划区信息||false|array|Zone|
|&emsp;&emsp;&emsp;&emsp;points|||false|array|PointDto|
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;x|||false|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;y|||false|integer||
|&emsp;&emsp;&emsp;&emsp;zoneId|||false|integer||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除任务


**接口地址**:`/nebula/task/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||
|iotId|设备id|query|true|string||
|type|类型|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除普通任务


**接口地址**:`/nebula/task/delete/combine`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除普通任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 定时任务开启或关闭


**接口地址**:`/nebula/task/execute`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>定时任务开启或关闭</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||
|isEnable|是否开启|query|true|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 导出任务列表


**接口地址**:`/nebula/task/export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>导出任务列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名|query|false|string||
|mapId|地图id|query|false|string||
|projectName|项目名|query|false|string||
|taskName|任务名称|query|false|string||
|taskType|任务类型,0普通任务,1定时任务|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 根据id获取普通任务


**接口地址**:`/nebula/task/getCombine`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据id获取普通任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|taskId|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据设备id获取组合任务


**接口地址**:`/nebula/task/getCombines`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据设备id获取组合任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备id|query|true|string||
|mapId|地图id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据id获取一个定时任务


**接口地址**:`/nebula/task/getTimer`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据id获取一个定时任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||
|iotId|设备id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据设备id获取定时任务


**接口地址**:`/nebula/task/getTimers`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据设备id获取定时任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 分页列表查询


**接口地址**:`/nebula/task/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|deviceName|设备名|query|false|string||
|mapId|地图id|query|false|string||
|projectName|项目名|query|false|string||
|taskName|任务名|query|false|string||
|taskType|任务类型,0普通任务,1定时任务|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«TaskPageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«TaskPageVo»|IPage«TaskPageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|TaskPageVo|
|&emsp;&emsp;&emsp;&emsp;cron|cron表达式|string||
|&emsp;&emsp;&emsp;&emsp;deviceName|设备名称|string||
|&emsp;&emsp;&emsp;&emsp;endDay|截止日|integer||
|&emsp;&emsp;&emsp;&emsp;endMonth|截止月|integer||
|&emsp;&emsp;&emsp;&emsp;endYear|截止年|integer||
|&emsp;&emsp;&emsp;&emsp;executeTaskName|执行任务名称|string||
|&emsp;&emsp;&emsp;&emsp;id||string||
|&emsp;&emsp;&emsp;&emsp;iotId|设备ID|string||
|&emsp;&emsp;&emsp;&emsp;isExecute|是否开启|boolean||
|&emsp;&emsp;&emsp;&emsp;isNever|True 不结束 False 截止日期|integer||
|&emsp;&emsp;&emsp;&emsp;mapId|地图id|string||
|&emsp;&emsp;&emsp;&emsp;mode|模式|integer||
|&emsp;&emsp;&emsp;&emsp;nextTime|下次cron表达式|string||
|&emsp;&emsp;&emsp;&emsp;nickName|备注名|string||
|&emsp;&emsp;&emsp;&emsp;projectName|项目名|string||
|&emsp;&emsp;&emsp;&emsp;rainSnow|雨雪天模式|boolean||
|&emsp;&emsp;&emsp;&emsp;rate|执行次数|integer||
|&emsp;&emsp;&emsp;&emsp;taskId|数据的id|string||
|&emsp;&emsp;&emsp;&emsp;taskName|任务名|string||
|&emsp;&emsp;&emsp;&emsp;taskType|任务类型 0普通   1定时|integer||
|&emsp;&emsp;&emsp;&emsp;type|类型|integer||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"cron": "",
				"deviceName": "",
				"endDay": 0,
				"endMonth": 0,
				"endYear": 0,
				"executeTaskName": "",
				"id": "",
				"iotId": "",
				"isExecute": true,
				"isNever": 0,
				"mapId": "",
				"mode": 0,
				"nextTime": "",
				"nickName": "",
				"projectName": "",
				"rainSnow": true,
				"rate": 0,
				"taskId": "",
				"taskName": "",
				"taskType": 0,
				"type": 0
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 设置雨雪天模式


**接口地址**:`/nebula/task/saveRainSnow`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>设置雨雪天模式</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||
|isEnable|是否开启|query|true|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 定时任务详情


**接口地址**:`/nebula/task/timerDetail`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>定时任务详情</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|date|日期，格式yyyy-MM-dd|query|true|string||
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 更新定时任务


**接口地址**:`/nebula/task/update`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>更新定时任务</p>



**请求示例**:


```javascript
{
  "deviceName": "",
  "endDay": 0,
  "endMonth": 0,
  "endYear": 0,
  "id": "",
  "iotId": "",
  "isExecute": 0,
  "isNever": 0,
  "isSkip": 0,
  "name": "",
  "omapId": "",
  "rate": 0,
  "taskId": 0,
  "taskName": "",
  "timerId": "",
  "timerRule": "",
  "type": 0
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|taskDto|taskDto|body|true|TaskDto|TaskDto|
|&emsp;&emsp;deviceName|设备名||false|string||
|&emsp;&emsp;endDay|日||false|integer(int32)||
|&emsp;&emsp;endMonth|月||false|integer(int32)||
|&emsp;&emsp;endYear|年||false|integer(int32)||
|&emsp;&emsp;id|主键||false|string||
|&emsp;&emsp;iotId|设备id||false|string||
|&emsp;&emsp;isExecute|是否开启||false|integer(int32)||
|&emsp;&emsp;isNever|截止日期||false|integer(int32)||
|&emsp;&emsp;isSkip|跳过节假日||false|integer(int32)||
|&emsp;&emsp;name|任务名称||false|string||
|&emsp;&emsp;omapId|||false|string||
|&emsp;&emsp;rate|执行频率||false|integer(int32)||
|&emsp;&emsp;taskId|组合任务id||false|integer(int64)||
|&emsp;&emsp;taskName|组合任务name||false|string||
|&emsp;&emsp;timerId|定时任务id||false|string||
|&emsp;&emsp;timerRule|规则||false|string||
|&emsp;&emsp;type|类型||false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 修改普通任务


**接口地址**:`/nebula/task/update/combine`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>修改普通任务</p>



**请求示例**:


```javascript
{
  "aromatherapyType": 0,
  "cleanType": 0,
  "id": "",
  "iotId": "",
  "mode": 0,
  "name": "",
  "omapId": "",
  "rate": 0,
  "zones": [
    {
      "points": [
        {
          "x": 0,
          "y": 0
        }
      ],
      "zoneId": 0
    }
  ]
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|createCombineDto|createCombineDto|body|true|CreateCombineDto|CreateCombineDto|
|&emsp;&emsp;aromatherapyType|香薰/消杀 0关闭，1消杀，3香薰||false|integer(int32)||
|&emsp;&emsp;cleanType|清洁模式 0关闭，1清扫，2湿拖，3尘吸，4尘推，5扫吸和尘推||false|integer(int32)||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;iotId|iotId||false|string||
|&emsp;&emsp;mode|0 Zoned 划区,1 Cover 全覆盖,2 Subregion 分区,3 Line 沿边||false|integer(int32)||
|&emsp;&emsp;name|任务名称||false|string||
|&emsp;&emsp;omapId|||false|string||
|&emsp;&emsp;rate|次数||false|integer(int32)||
|&emsp;&emsp;zones|划区信息||false|array|Zone|
|&emsp;&emsp;&emsp;&emsp;points|||false|array|PointDto|
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;x|||false|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;y|||false|integer||
|&emsp;&emsp;&emsp;&emsp;zoneId|||false|integer||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 工作计划


**接口地址**:`/nebula/task/workPlan`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>工作计划</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|date|日期，格式yyyy-MM-dd|query|true|string||
|deviceName|设备编号|query|true|string||
|mapId|地图id|query|false|string||
|type|类型，,月month,周week|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 单表DEMO


## 添加DEMO


**接口地址**:`/nebula/test/jeecgDemo/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>添加DEMO</p>



**请求示例**:


```javascript
{
  "age": 0,
  "birthday": "",
  "bonusMoney": 0,
  "content": "",
  "createBy": "",
  "createTime": "",
  "email": "",
  "id": "",
  "keyWord": "",
  "name": "",
  "punchTime": "",
  "salaryMoney": 0,
  "sex": "",
  "sysOrgCode": "",
  "tenantId": 0,
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|jeecgDemo|测试DEMO|body|true|测试DEMO对象|测试DEMO对象|
|&emsp;&emsp;age|年龄||false|integer(int32)||
|&emsp;&emsp;birthday|生日||false|string(date-time)||
|&emsp;&emsp;bonusMoney|奖金||false|number(double)||
|&emsp;&emsp;content|个人简介||false|string||
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;email|邮箱||false|string||
|&emsp;&emsp;id|ID||false|string||
|&emsp;&emsp;keyWord|关键词||false|string||
|&emsp;&emsp;name|姓名||false|string||
|&emsp;&emsp;punchTime|打卡时间||false|string(date-time)||
|&emsp;&emsp;salaryMoney|工资||false|number||
|&emsp;&emsp;sex|性别||false|string||
|&emsp;&emsp;sysOrgCode|部门编码||false|string||
|&emsp;&emsp;tenantId|租户ID||false|integer(int32)||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 通过ID删除DEMO


**接口地址**:`/nebula/test/jeecgDemo/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>通过ID删除DEMO</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 批量删除DEMO


**接口地址**:`/nebula/test/jeecgDemo/deleteBatch`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>批量删除DEMO</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|ids|ids|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 编辑DEMO


**接口地址**:`/nebula/test/jeecgDemo/edit`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>编辑DEMO</p>



**请求示例**:


```javascript
{
  "age": 0,
  "birthday": "",
  "bonusMoney": 0,
  "content": "",
  "createBy": "",
  "createTime": "",
  "email": "",
  "id": "",
  "keyWord": "",
  "name": "",
  "punchTime": "",
  "salaryMoney": 0,
  "sex": "",
  "sysOrgCode": "",
  "tenantId": 0,
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|jeecgDemo|测试DEMO|body|true|测试DEMO对象|测试DEMO对象|
|&emsp;&emsp;age|年龄||false|integer(int32)||
|&emsp;&emsp;birthday|生日||false|string(date-time)||
|&emsp;&emsp;bonusMoney|奖金||false|number(double)||
|&emsp;&emsp;content|个人简介||false|string||
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;email|邮箱||false|string||
|&emsp;&emsp;id|ID||false|string||
|&emsp;&emsp;keyWord|关键词||false|string||
|&emsp;&emsp;name|姓名||false|string||
|&emsp;&emsp;punchTime|打卡时间||false|string(date-time)||
|&emsp;&emsp;salaryMoney|工资||false|number||
|&emsp;&emsp;sex|性别||false|string||
|&emsp;&emsp;sysOrgCode|部门编码||false|string||
|&emsp;&emsp;tenantId|租户ID||false|integer(int32)||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 编辑DEMO


**接口地址**:`/nebula/test/jeecgDemo/edit`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>编辑DEMO</p>



**请求示例**:


```javascript
{
  "age": 0,
  "birthday": "",
  "bonusMoney": 0,
  "content": "",
  "createBy": "",
  "createTime": "",
  "email": "",
  "id": "",
  "keyWord": "",
  "name": "",
  "punchTime": "",
  "salaryMoney": 0,
  "sex": "",
  "sysOrgCode": "",
  "tenantId": 0,
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|jeecgDemo|测试DEMO|body|true|测试DEMO对象|测试DEMO对象|
|&emsp;&emsp;age|年龄||false|integer(int32)||
|&emsp;&emsp;birthday|生日||false|string(date-time)||
|&emsp;&emsp;bonusMoney|奖金||false|number(double)||
|&emsp;&emsp;content|个人简介||false|string||
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;email|邮箱||false|string||
|&emsp;&emsp;id|ID||false|string||
|&emsp;&emsp;keyWord|关键词||false|string||
|&emsp;&emsp;name|姓名||false|string||
|&emsp;&emsp;punchTime|打卡时间||false|string(date-time)||
|&emsp;&emsp;salaryMoney|工资||false|number||
|&emsp;&emsp;sex|性别||false|string||
|&emsp;&emsp;sysOrgCode|部门编码||false|string||
|&emsp;&emsp;tenantId|租户ID||false|integer(int32)||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取Demo数据列表


**接口地址**:`/nebula/test/jeecgDemo/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取所有Demo数据列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|age|年龄|query|false|integer(int32)||
|birthday|生日|query|false|string(date-time)||
|bonusMoney|奖金|query|false|number(double)||
|content|个人简介|query|false|string||
|createBy|创建人|query|false|string||
|createTime|创建时间|query|false|string(date-time)||
|email|邮箱|query|false|string||
|id|ID|query|false|string||
|keyWord|关键词|query|false|string||
|name|姓名|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|punchTime|打卡时间|query|false|string(date-time)||
|salaryMoney|工资|query|false|number||
|sex|性别|query|false|string||
|sysOrgCode|部门编码|query|false|string||
|tenantId|租户ID|query|false|integer(int32)||
|updateBy|更新人|query|false|string||
|updateTime|更新时间|query|false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 通过ID查询DEMO


**接口地址**:`/nebula/test/jeecgDemo/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>通过ID查询DEMO</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|示例id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 在线留言


## 新建留言


**接口地址**:`/nebula/online/message/create`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>新建留言</p>



**请求示例**:


```javascript
{
  "message": "",
  "phone": "",
  "username": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|onlineMessageDto|onlineMessageDto|body|true|OnlineMessageDto|OnlineMessageDto|
|&emsp;&emsp;message|留言||false|string||
|&emsp;&emsp;phone|电话||false|string||
|&emsp;&emsp;username|姓名||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 地图快照


## 获取地图快照


**接口地址**:`/nebula/map/snapshot/findById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取地图快照</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|integer(int64)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 填值规则


## 填值规则-添加


**接口地址**:`/nebula/sys/fillRule/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>填值规则-添加</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "id": "",
  "ruleClass": "",
  "ruleCode": "",
  "ruleName": "",
  "ruleParams": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysFillRule|填值规则|body|true|sys_fill_rule对象|sys_fill_rule对象|
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;id|主键ID||false|string||
|&emsp;&emsp;ruleClass|规则实现类||false|string||
|&emsp;&emsp;ruleCode|规则Code||false|string||
|&emsp;&emsp;ruleName|规则名称||false|string||
|&emsp;&emsp;ruleParams|规则参数||false|string||
|&emsp;&emsp;updateBy|修改人||false|string||
|&emsp;&emsp;updateTime|修改时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 填值规则-通过id删除


**接口地址**:`/nebula/sys/fillRule/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>填值规则-通过id删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 填值规则-批量删除


**接口地址**:`/nebula/sys/fillRule/deleteBatch`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>填值规则-批量删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|ids|ids|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 填值规则-编辑


**接口地址**:`/nebula/sys/fillRule/edit`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>填值规则-编辑</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "id": "",
  "ruleClass": "",
  "ruleCode": "",
  "ruleName": "",
  "ruleParams": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysFillRule|填值规则|body|true|sys_fill_rule对象|sys_fill_rule对象|
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;id|主键ID||false|string||
|&emsp;&emsp;ruleClass|规则实现类||false|string||
|&emsp;&emsp;ruleCode|规则Code||false|string||
|&emsp;&emsp;ruleName|规则名称||false|string||
|&emsp;&emsp;ruleParams|规则参数||false|string||
|&emsp;&emsp;updateBy|修改人||false|string||
|&emsp;&emsp;updateTime|修改时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 填值规则-编辑


**接口地址**:`/nebula/sys/fillRule/edit`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>填值规则-编辑</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "id": "",
  "ruleClass": "",
  "ruleCode": "",
  "ruleName": "",
  "ruleParams": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysFillRule|填值规则|body|true|sys_fill_rule对象|sys_fill_rule对象|
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;id|主键ID||false|string||
|&emsp;&emsp;ruleClass|规则实现类||false|string||
|&emsp;&emsp;ruleCode|规则Code||false|string||
|&emsp;&emsp;ruleName|规则名称||false|string||
|&emsp;&emsp;ruleParams|规则参数||false|string||
|&emsp;&emsp;updateBy|修改人||false|string||
|&emsp;&emsp;updateTime|修改时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 填值规则-分页列表查询


**接口地址**:`/nebula/sys/fillRule/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>填值规则-分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|createBy|创建人|query|false|string||
|createTime|创建时间|query|false|string(date-time)||
|id|主键ID|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|ruleClass|规则实现类|query|false|string||
|ruleCode|规则Code|query|false|string||
|ruleName|规则名称|query|false|string||
|ruleParams|规则参数|query|false|string||
|updateBy|修改人|query|false|string||
|updateTime|修改时间|query|false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 填值规则-通过id查询


**接口地址**:`/nebula/sys/fillRule/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>填值规则-通过id查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 多数据源管理


## 多数据源管理-添加


**接口地址**:`/nebula/sys/dataSource/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>多数据源管理-添加</p>



**请求示例**:


```javascript
{
  "code": "",
  "createBy": "",
  "createTime": "",
  "dbDriver": "",
  "dbName": "",
  "dbPassword": "",
  "dbType": "",
  "dbUrl": "",
  "dbUsername": "",
  "id": "",
  "name": "",
  "remark": "",
  "sysOrgCode": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysDataSource|多数据源管理|body|true|sys_data_source对象|sys_data_source对象|
|&emsp;&emsp;code|数据源编码||false|string||
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建日期||false|string(date-time)||
|&emsp;&emsp;dbDriver|驱动类||false|string||
|&emsp;&emsp;dbName|数据库名称||false|string||
|&emsp;&emsp;dbPassword|密码||false|string||
|&emsp;&emsp;dbType|数据库类型||false|string||
|&emsp;&emsp;dbUrl|数据源地址||false|string||
|&emsp;&emsp;dbUsername|用户名||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;name|数据源名称||false|string||
|&emsp;&emsp;remark|备注||false|string||
|&emsp;&emsp;sysOrgCode|所属部门||false|string||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新日期||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 多数据源管理-通过id删除


**接口地址**:`/nebula/sys/dataSource/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>多数据源管理-通过id删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 多数据源管理-批量删除


**接口地址**:`/nebula/sys/dataSource/deleteBatch`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>多数据源管理-批量删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|ids|ids|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 多数据源管理-编辑


**接口地址**:`/nebula/sys/dataSource/edit`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>多数据源管理-编辑</p>



**请求示例**:


```javascript
{
  "code": "",
  "createBy": "",
  "createTime": "",
  "dbDriver": "",
  "dbName": "",
  "dbPassword": "",
  "dbType": "",
  "dbUrl": "",
  "dbUsername": "",
  "id": "",
  "name": "",
  "remark": "",
  "sysOrgCode": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysDataSource|多数据源管理|body|true|sys_data_source对象|sys_data_source对象|
|&emsp;&emsp;code|数据源编码||false|string||
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建日期||false|string(date-time)||
|&emsp;&emsp;dbDriver|驱动类||false|string||
|&emsp;&emsp;dbName|数据库名称||false|string||
|&emsp;&emsp;dbPassword|密码||false|string||
|&emsp;&emsp;dbType|数据库类型||false|string||
|&emsp;&emsp;dbUrl|数据源地址||false|string||
|&emsp;&emsp;dbUsername|用户名||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;name|数据源名称||false|string||
|&emsp;&emsp;remark|备注||false|string||
|&emsp;&emsp;sysOrgCode|所属部门||false|string||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新日期||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 多数据源管理-编辑


**接口地址**:`/nebula/sys/dataSource/edit`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>多数据源管理-编辑</p>



**请求示例**:


```javascript
{
  "code": "",
  "createBy": "",
  "createTime": "",
  "dbDriver": "",
  "dbName": "",
  "dbPassword": "",
  "dbType": "",
  "dbUrl": "",
  "dbUsername": "",
  "id": "",
  "name": "",
  "remark": "",
  "sysOrgCode": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysDataSource|多数据源管理|body|true|sys_data_source对象|sys_data_source对象|
|&emsp;&emsp;code|数据源编码||false|string||
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建日期||false|string(date-time)||
|&emsp;&emsp;dbDriver|驱动类||false|string||
|&emsp;&emsp;dbName|数据库名称||false|string||
|&emsp;&emsp;dbPassword|密码||false|string||
|&emsp;&emsp;dbType|数据库类型||false|string||
|&emsp;&emsp;dbUrl|数据源地址||false|string||
|&emsp;&emsp;dbUsername|用户名||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;name|数据源名称||false|string||
|&emsp;&emsp;remark|备注||false|string||
|&emsp;&emsp;sysOrgCode|所属部门||false|string||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新日期||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 多数据源管理-分页列表查询


**接口地址**:`/nebula/sys/dataSource/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>多数据源管理-分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|code|数据源编码|query|false|string||
|createBy|创建人|query|false|string||
|createTime|创建日期|query|false|string(date-time)||
|dbDriver|驱动类|query|false|string||
|dbName|数据库名称|query|false|string||
|dbPassword|密码|query|false|string||
|dbType|数据库类型|query|false|string||
|dbUrl|数据源地址|query|false|string||
|dbUsername|用户名|query|false|string||
|id|id|query|false|string||
|name|数据源名称|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|remark|备注|query|false|string||
|sysOrgCode|所属部门|query|false|string||
|updateBy|更新人|query|false|string||
|updateTime|更新日期|query|false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 多数据源管理-通过id查询


**接口地址**:`/nebula/sys/dataSource/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>多数据源管理-通过id查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 定时任务接口


## 停止定时任务


**接口地址**:`/nebula/sys/quartzJob/pause`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 启动定时任务


**接口地址**:`/nebula/sys/quartzJob/resume`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 对外接口


## 每天清洁记录


**接口地址**:`/nebula/api/v1/device/clean/day/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>每天清洁记录</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备id|query|true|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«Page«CleanRecordVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|Page«CleanRecordVo»|Page«CleanRecordVo»|
|&emsp;&emsp;countId||string||
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;maxLimit||integer(int64)||
|&emsp;&emsp;optimizeCountSql||boolean||
|&emsp;&emsp;orders||array|OrderItem|
|&emsp;&emsp;&emsp;&emsp;asc||boolean||
|&emsp;&emsp;&emsp;&emsp;column||string||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|CleanRecordVo|
|&emsp;&emsp;&emsp;&emsp;cleanArea|清洁面积|string||
|&emsp;&emsp;&emsp;&emsp;cleanTaskVos|记录|array|CleanTaskVo|
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;aromatherapyStatus|香薰|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;cleanArea|清扫面积|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;cleanTime|清扫时长|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;deviceName|设备名称|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;disinfectStatus|消杀|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;endTime|结束时间|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;errMsg|异常信息|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;errorCode|错误码|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;errorMessage|任务错误信息码|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;executeDate|日期|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;executeTime|开始时间|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;iotId|设备Id|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;isComplete||integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;isSuccess|任务是否成功码|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;mopStatus|湿拖|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;operEvent|事件|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;planId|对应的执行规划任务id|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;pushStatus|尘推|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;snapshotId|快照id|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;startMode|启动方式|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;sweepStatus|扫头|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;taskId|任务Id|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;taskMode|任务模式|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;taskName|任务名称|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;taskType|任务类型|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;vacuumStatus|风机|integer||
|&emsp;&emsp;&emsp;&emsp;cleanTime|清扫时长|string||
|&emsp;&emsp;&emsp;&emsp;executeDate|日期|string||
|&emsp;&emsp;&emsp;&emsp;taskNum|任务次数|string||
|&emsp;&emsp;searchCount||boolean||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"countId": "",
		"current": 0,
		"maxLimit": 0,
		"optimizeCountSql": true,
		"orders": [
			{
				"asc": true,
				"column": ""
			}
		],
		"pages": 0,
		"records": [
			{
				"cleanArea": "",
				"cleanTaskVos": [
					{
						"aromatherapyStatus": 0,
						"cleanArea": "",
						"cleanTime": "",
						"deviceName": "",
						"disinfectStatus": 0,
						"endTime": "",
						"errMsg": "",
						"errorCode": "",
						"errorMessage": "",
						"executeDate": "",
						"executeTime": "",
						"id": "",
						"iotId": "",
						"isComplete": 0,
						"isSuccess": 0,
						"mopStatus": 0,
						"operEvent": "",
						"planId": 0,
						"pushStatus": 0,
						"snapshotId": "",
						"startMode": "",
						"sweepStatus": 0,
						"taskId": "",
						"taskMode": 0,
						"taskName": "",
						"taskType": 0,
						"vacuumStatus": 0
					}
				],
				"cleanTime": "",
				"executeDate": "",
				"taskNum": ""
			}
		],
		"searchCount": true,
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 获取设备地图的禁区


**接口地址**:`/nebula/api/v1/device/map/exclusion`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备地图的禁区</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备ID|query|true|string||
|mapId|地图ID|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«MapInfoWallVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array|MapInfoWallVo|
|&emsp;&emsp;point||array|ModelCleanAreaEventPeakDto|
|&emsp;&emsp;&emsp;&emsp;x||number||
|&emsp;&emsp;&emsp;&emsp;y||number||
|&emsp;&emsp;type||integer(int32)||
|&emsp;&emsp;zoneId||integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [
		{
			"point": [
				{
					"x": 0,
					"y": 0
				}
			],
			"type": 0,
			"zoneId": 0
		}
	],
	"success": true,
	"timestamp": 0
}
```


## 获取设备地图的虚拟墙


**接口地址**:`/nebula/api/v1/device/map/virtual/wall`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备地图的虚拟墙</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备ID|query|true|string||
|mapId|地图ID|query|false|||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«MapInfoWallVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array|MapInfoWallVo|
|&emsp;&emsp;point||array|ModelCleanAreaEventPeakDto|
|&emsp;&emsp;&emsp;&emsp;x||number||
|&emsp;&emsp;&emsp;&emsp;y||number||
|&emsp;&emsp;type||integer(int32)||
|&emsp;&emsp;zoneId||integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [
		{
			"point": [
				{
					"x": 0,
					"y": 0
				}
			],
			"type": 0,
			"zoneId": 0
		}
	],
	"success": true,
	"timestamp": 0
}
```


## 执行任务


**接口地址**:`/nebula/api/v1/device/performTask`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>执行任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|executeNum|执行次数|query|true|integer(int32)||
|taskId|任务id|query|true|integer(int64)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取机器人实时位姿


**接口地址**:`/nebula/api/v1/device/real/pose`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取机器人实时位姿</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取设备状态


**接口地址**:`/nebula/api/v1/device/status`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备状态</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«DeviceInfoVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|DeviceInfoVo|DeviceInfoVo|
|&emsp;&emsp;absorbStatus|扫吸状态|string||
|&emsp;&emsp;cleanWaterBoxStatus|清水箱余水量，取值百分比（%）|string||
|&emsp;&emsp;deviceName|设备编号|string||
|&emsp;&emsp;deviceStatus|设备状态|string||
|&emsp;&emsp;dirtyWaterBoxStatus|污水箱状态。0：未满，1：满|string||
|&emsp;&emsp;electric|电池电量|string||
|&emsp;&emsp;id||string||
|&emsp;&emsp;nickname|名称|string||
|&emsp;&emsp;projectName|项目名称|string||
|&emsp;&emsp;pushStatus|尘推状态|string||
|&emsp;&emsp;sweepStatus|湿拖状态|string||
|&emsp;&emsp;workStatus|工作状态|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"absorbStatus": "",
		"cleanWaterBoxStatus": "",
		"deviceName": "",
		"deviceStatus": "",
		"dirtyWaterBoxStatus": "",
		"electric": "",
		"id": "",
		"nickname": "",
		"projectName": "",
		"pushStatus": "",
		"sweepStatus": "",
		"workStatus": ""
	},
	"success": true,
	"timestamp": 0
}
```


# 小程序


## 小程序用户考试认证


**接口地址**:`/nebula/wxa/auth/apply`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>小程序用户考试认证</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 小程序用户考试认证状态


**接口地址**:`/nebula/wxa/auth/apply/status`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>小程序用户考试认证状态</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«CertificateStatusVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|CertificateStatusVo|CertificateStatusVo|
|&emsp;&emsp;res|认证结果1 CRO 2 CRP 3 CRE|integer(int32)||
|&emsp;&emsp;status| 0未提交  1已提交未认证  2已提交已认证|integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"res": 0,
		"status": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 获取设备票据


**接口地址**:`/nebula/wxa/getSnticket`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备票据</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sn|设备唯一序列号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«SnTicketVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|SnTicketVo|SnTicketVo|
|&emsp;&emsp;snTicket|设备票据，5分钟内有效。|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"snTicket": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 获取模板列表


**接口地址**:`/nebula/wxa/newtmpl/gettemplate`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取模板列表</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 保存长期订阅用户


**接口地址**:`/nebula/wxa/newtmpl/savefollowers`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>保存长期订阅用户</p>



**请求示例**:


```javascript
{
  "openId": "",
  "templateIds": []
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|miniappFollowerDto|miniappFollowerDto|body|true|MiniappFollowerDto|MiniappFollowerDto|
|&emsp;&emsp;openId|||false|string||
|&emsp;&emsp;templateIds|||false|array|string|


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 同步个人模板列表


**接口地址**:`/nebula/wxa/newtmpl/synctemplate`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>同步个人模板列表</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 微信消息通知


## 申请权限


**接口地址**:`/nebula/wx/message/applyPermission`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>申请权限</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 详情


**接口地址**:`/nebula/wx/message/detail`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>详情</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|消息id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 反馈消息


**接口地址**:`/nebula/wx/message/feedback`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>反馈消息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|消息id|query|true|integer(int64)||
|res|结果类型 0同意  1拒绝|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 消息列表


**接口地址**:`/nebula/wx/message/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>消息列表</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«WxMessageNoticeVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array|WxMessageNoticeVo|
|&emsp;&emsp;content|内容|string||
|&emsp;&emsp;createTime|创建时间|integer(int64)||
|&emsp;&emsp;status|状态 true未处理，false已处理|boolean||
|&emsp;&emsp;title|标题|string||
|&emsp;&emsp;type|消息类型 1系统消息|integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [
		{
			"content": "",
			"createTime": 0,
			"status": true,
			"title": "",
			"type": 0
		}
	],
	"success": true,
	"timestamp": 0
}
```


## 分页列表查询


**接口地址**:`/nebula/wx/message/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|type|每页显示数|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«WxMessagePageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«WxMessagePageVo»|IPage«WxMessagePageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|WxMessagePageVo|
|&emsp;&emsp;&emsp;&emsp;content|内容|string||
|&emsp;&emsp;&emsp;&emsp;createTime|创建时间|integer||
|&emsp;&emsp;&emsp;&emsp;detailType|详细消息类型 1设备共享|integer||
|&emsp;&emsp;&emsp;&emsp;id|主键|string||
|&emsp;&emsp;&emsp;&emsp;status|状态 0未处理，1已处理|integer||
|&emsp;&emsp;&emsp;&emsp;title|标题|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"content": "",
				"createTime": 0,
				"detailType": 0,
				"id": "",
				"status": 0,
				"title": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 查询信息


**接口地址**:`/nebula/wx/message/query`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询信息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|消息id|query|true|integer(int64)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«WxMessageVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|WxMessageVo|WxMessageVo|
|&emsp;&emsp;isHandle|是否需要处理 0不需要  1需要|integer(int32)||
|&emsp;&emsp;res|结果类型 0同意  1拒绝|integer(int32)||
|&emsp;&emsp;status|状态 0未处理，1已处理|integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"isHandle": 0,
		"res": 0,
		"status": 0
	},
	"success": true,
	"timestamp": 0
}
```


# 意见消息


## 新建意见


**接口地址**:`/nebula/opinion/create`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>新建意见</p>



**请求示例**:


```javascript
{
  "content": "",
  "deviceName": "",
  "file": [
    {
      "fileName": "",
      "fileType": 0
    }
  ],
  "opinionType": 0,
  "phone": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|opinionCreateDto|opinionCreateDto|body|true|OpinionCreateDto|OpinionCreateDto|
|&emsp;&emsp;content|反馈内容 200字以内||false|string||
|&emsp;&emsp;deviceName|设备名||false|string||
|&emsp;&emsp;file|附件||false|array|OpinionFileDto|
|&emsp;&emsp;&emsp;&emsp;fileName|附件名||false|string||
|&emsp;&emsp;&emsp;&emsp;fileType|附件类型 1图片  2视频||false|integer||
|&emsp;&emsp;opinionType|意见类型||false|integer(int32)||
|&emsp;&emsp;phone|电话||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 回复意见


**接口地址**:`/nebula/opinion/feedback`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>回复意见</p>



**请求示例**:


```javascript
{
  "feedbackContent": "",
  "id": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|opinionFeedbackDto|opinionFeedbackDto|body|true|OpinionFeedbackDto|OpinionFeedbackDto|
|&emsp;&emsp;feedbackContent|反馈内容 200字以内||false|string||
|&emsp;&emsp;id|id||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 分页列表查询


**接口地址**:`/nebula/opinion/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|opinionType|意见类型|query|false|integer(int32)||
|replyType|状态0未回复1已回复|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«OpinionMessagePageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«OpinionMessagePageVo»|IPage«OpinionMessagePageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|OpinionMessagePageVo|
|&emsp;&emsp;&emsp;&emsp;content|反馈内容|string||
|&emsp;&emsp;&emsp;&emsp;createTime|创建时间|integer||
|&emsp;&emsp;&emsp;&emsp;deviceName|设备名|string||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;opinionType|意见类型|integer||
|&emsp;&emsp;&emsp;&emsp;phone|手机号|string||
|&emsp;&emsp;&emsp;&emsp;replyType|回复状态|boolean||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"content": "",
				"createTime": 0,
				"deviceName": "",
				"id": "",
				"opinionType": 0,
				"phone": "",
				"replyType": true
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 查看单条消息


**接口地址**:`/nebula/opinion/query`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«OpinionMessageVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|OpinionMessageVo|OpinionMessageVo|
|&emsp;&emsp;content|反馈内容|string||
|&emsp;&emsp;createTime|创建时间|integer(int64)||
|&emsp;&emsp;deviceName|设备名|string||
|&emsp;&emsp;feedbackContent|回复内容|string||
|&emsp;&emsp;feedbackTime|回复时间|integer(int64)||
|&emsp;&emsp;file|附件|array|OpinionFileVo|
|&emsp;&emsp;&emsp;&emsp;fileType|附件类型 1图片  2视频|integer||
|&emsp;&emsp;&emsp;&emsp;fileUrl|附件签名|string||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;opinionType|意见类型|integer(int32)||
|&emsp;&emsp;phone|手机号|string||
|&emsp;&emsp;replyType|回复状态|boolean||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"content": "",
		"createTime": 0,
		"deviceName": "",
		"feedbackContent": "",
		"feedbackTime": 0,
		"file": [
			{
				"fileType": 0,
				"fileUrl": ""
			}
		],
		"id": "",
		"opinionType": 0,
		"phone": "",
		"replyType": true
	},
	"success": true,
	"timestamp": 0
}
```


## 小程序我的反馈


**接口地址**:`/nebula/opinion/wx-page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>小程序我的反馈</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«OpinionMessagePageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«OpinionMessagePageVo»|IPage«OpinionMessagePageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|OpinionMessagePageVo|
|&emsp;&emsp;&emsp;&emsp;content|反馈内容|string||
|&emsp;&emsp;&emsp;&emsp;createTime|创建时间|integer||
|&emsp;&emsp;&emsp;&emsp;deviceName|设备名|string||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;opinionType|意见类型|integer||
|&emsp;&emsp;&emsp;&emsp;phone|手机号|string||
|&emsp;&emsp;&emsp;&emsp;replyType|回复状态|boolean||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"content": "",
				"createTime": 0,
				"deviceName": "",
				"id": "",
				"opinionType": 0,
				"phone": "",
				"replyType": true
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


# 数据字典


## 字典重复校验接口


**接口地址**:`/nebula/sys/dictItem/dictItemCheck`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|createBy||query|false|string||
|createTime||query|false|string(date-time)||
|description||query|false|string||
|dictId||query|false|string||
|id||query|false|string||
|itemText||query|false|string||
|itemValue||query|false|string||
|sortOrder||query|false|integer(int32)||
|status||query|false|integer(int32)||
|updateBy||query|false|string||
|updateTime||query|false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 清洁任务


## 每天清洁记录


**接口地址**:`/nebula/cleantask/clean-record-day-page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>每天清洁记录</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备id|query|true|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«Page«CleanRecordVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|Page«CleanRecordVo»|Page«CleanRecordVo»|
|&emsp;&emsp;countId||string||
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;maxLimit||integer(int64)||
|&emsp;&emsp;optimizeCountSql||boolean||
|&emsp;&emsp;orders||array|OrderItem|
|&emsp;&emsp;&emsp;&emsp;asc||boolean||
|&emsp;&emsp;&emsp;&emsp;column||string||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|CleanRecordVo|
|&emsp;&emsp;&emsp;&emsp;cleanArea|清洁面积|string||
|&emsp;&emsp;&emsp;&emsp;cleanTaskVos|记录|array|CleanTaskVo|
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;aromatherapyStatus|香薰|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;cleanArea|清扫面积|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;cleanTime|清扫时长|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;deviceName|设备名称|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;disinfectStatus|消杀|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;endTime|结束时间|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;errMsg|异常信息|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;errorCode|错误码|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;errorMessage|任务错误信息码|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;executeDate|日期|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;executeTime|开始时间|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;iotId|设备Id|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;isComplete||integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;isSuccess|任务是否成功码|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;mopStatus|湿拖|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;operEvent|事件|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;planId|对应的执行规划任务id|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;pushStatus|尘推|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;snapshotId|快照id|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;startMode|启动方式|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;sweepStatus|扫头|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;taskId|任务Id|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;taskMode|任务模式|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;taskName|任务名称|string||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;taskType|任务类型|integer||
|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;vacuumStatus|风机|integer||
|&emsp;&emsp;&emsp;&emsp;cleanTime|清扫时长|string||
|&emsp;&emsp;&emsp;&emsp;executeDate|日期|string||
|&emsp;&emsp;&emsp;&emsp;taskNum|任务次数|string||
|&emsp;&emsp;searchCount||boolean||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"countId": "",
		"current": 0,
		"maxLimit": 0,
		"optimizeCountSql": true,
		"orders": [
			{
				"asc": true,
				"column": ""
			}
		],
		"pages": 0,
		"records": [
			{
				"cleanArea": "",
				"cleanTaskVos": [
					{
						"aromatherapyStatus": 0,
						"cleanArea": "",
						"cleanTime": "",
						"deviceName": "",
						"disinfectStatus": 0,
						"endTime": "",
						"errMsg": "",
						"errorCode": "",
						"errorMessage": "",
						"executeDate": "",
						"executeTime": "",
						"id": "",
						"iotId": "",
						"isComplete": 0,
						"isSuccess": 0,
						"mopStatus": 0,
						"operEvent": "",
						"planId": 0,
						"pushStatus": 0,
						"snapshotId": "",
						"startMode": "",
						"sweepStatus": 0,
						"taskId": "",
						"taskMode": 0,
						"taskName": "",
						"taskType": 0,
						"vacuumStatus": 0
					}
				],
				"cleanTime": "",
				"executeDate": "",
				"taskNum": ""
			}
		],
		"searchCount": true,
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 清洁任务详情


**接口地址**:`/nebula/cleantask/clean-task-detail`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>清洁任务详情</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|cleanTaskId|清洁任务Id|query|true|string||
|isSuccess|清洁任务是否完成|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«CleanTaskDetailVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|CleanTaskDetailVo|CleanTaskDetailVo|
|&emsp;&emsp;cleanArea|清扫面积|integer(int32)||
|&emsp;&emsp;cleanTime|清扫时长|integer(int32)||
|&emsp;&emsp;createTime|创建时间|string(date-time)||
|&emsp;&emsp;endTime|结束时间|string||
|&emsp;&emsp;errorCode|错误码|string||
|&emsp;&emsp;errorMessage|任务错误信息|string||
|&emsp;&emsp;executeTime|执行日期|string||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;isSuccess|任务是否成功码|integer(int32)||
|&emsp;&emsp;mapId|地图id|string||
|&emsp;&emsp;mapName|地图Name|string||
|&emsp;&emsp;name|设备名称|string||
|&emsp;&emsp;operEvent|手动事件|string||
|&emsp;&emsp;planId|对应的执行规划任务id|integer(int32)||
|&emsp;&emsp;rate|执行次数|integer(int32)||
|&emsp;&emsp;startMode|任务类型|string||
|&emsp;&emsp;taskId|任务Id|string||
|&emsp;&emsp;taskMode|任务模式|integer(int32)||
|&emsp;&emsp;taskName|任务名称|string||
|&emsp;&emsp;taskTimes|任务应执行次数|integer(int32)||
|&emsp;&emsp;taskType|启动方式|integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"cleanArea": 0,
		"cleanTime": 0,
		"createTime": "",
		"endTime": "",
		"errorCode": "",
		"errorMessage": "",
		"executeTime": "",
		"id": "",
		"isSuccess": 0,
		"mapId": "",
		"mapName": "",
		"name": "",
		"operEvent": "",
		"planId": 0,
		"rate": 0,
		"startMode": "",
		"taskId": "",
		"taskMode": 0,
		"taskName": "",
		"taskTimes": 0,
		"taskType": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 清洁任务信息


**接口地址**:`/nebula/cleantask/clean/task/info`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>清洁任务信息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|cleanTaskId|清洁任务Id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«CleanTaskUncompletedVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|CleanTaskUncompletedVo|CleanTaskUncompletedVo|
|&emsp;&emsp;aromatherapyStatus|香薰状态|integer(int32)||
|&emsp;&emsp;cleanArea|清扫面积|string||
|&emsp;&emsp;cleanTime|清扫时长|string||
|&emsp;&emsp;departName|项目名称|string||
|&emsp;&emsp;deviceName|设备编号|string||
|&emsp;&emsp;deviceRosVersion|软件版本|string||
|&emsp;&emsp;disinfectStatus|消杀状态|integer(int32)||
|&emsp;&emsp;endTime|结束时间|string(date-time)||
|&emsp;&emsp;errMsg|异常信息|string||
|&emsp;&emsp;errorMessage|异常错误码|string||
|&emsp;&emsp;executeTime|开始时间|string(date-time)||
|&emsp;&emsp;fileLinkVos|文件|array|FileLinkVo|
|&emsp;&emsp;&emsp;&emsp;fileLink|bag链接|string||
|&emsp;&emsp;&emsp;&emsp;fileName|bag包名|string||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;mapId|地图id|string||
|&emsp;&emsp;mapName|地图Name|string||
|&emsp;&emsp;mopStatus|湿拖等级|integer(int32)||
|&emsp;&emsp;nickname|备注名|string||
|&emsp;&emsp;operEvent|手动事件|string||
|&emsp;&emsp;pushStatus|尘推等级|integer(int32)||
|&emsp;&emsp;startMode|任务类型|string||
|&emsp;&emsp;status|文件状态|integer(int32)||
|&emsp;&emsp;sweepStatus|扫头状态|integer(int32)||
|&emsp;&emsp;taskId|任务id|string||
|&emsp;&emsp;taskMode|任务模式|integer(int32)||
|&emsp;&emsp;taskType|启动方式|integer(int32)||
|&emsp;&emsp;vacuumStatus|风机等级|integer(int32)||
|&emsp;&emsp;valid|rosBag包的有效性状态 0 可以拉取 1可以下载 2 拉取中 3 不能拉取不能下载|integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"aromatherapyStatus": 0,
		"cleanArea": "",
		"cleanTime": "",
		"departName": "",
		"deviceName": "",
		"deviceRosVersion": "",
		"disinfectStatus": 0,
		"endTime": "",
		"errMsg": "",
		"errorMessage": "",
		"executeTime": "",
		"fileLinkVos": [
			{
				"fileLink": "",
				"fileName": ""
			}
		],
		"id": "",
		"mapId": "",
		"mapName": "",
		"mopStatus": 0,
		"nickname": "",
		"operEvent": "",
		"pushStatus": 0,
		"startMode": "",
		"status": 0,
		"sweepStatus": 0,
		"taskId": "",
		"taskMode": 0,
		"taskType": 0,
		"vacuumStatus": 0,
		"valid": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 清洁记录导出


**接口地址**:`/nebula/cleantask/export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>清洁记录导出</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名|query|true|string||
|iotId|设备id|query|true|string||
|isComplete|是否完成|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 清扫记录分页


**接口地址**:`/nebula/cleantask/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>清扫记录分页</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名|query|true|string||
|iotId|设备id|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|endDate|结束日期|query|false|string||
|isComplete|是否完成 1已完成 2未完成 3等待|query|false|integer(int32)||
|startDate|开始日期|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«CleanTask对象»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«CleanTask对象»|IPage«CleanTask对象»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|CleanTask对象|
|&emsp;&emsp;&emsp;&emsp;aromatherapyStatus|香薰状态|integer||
|&emsp;&emsp;&emsp;&emsp;baseComplete||integer||
|&emsp;&emsp;&emsp;&emsp;cleanArea|清扫面积|integer||
|&emsp;&emsp;&emsp;&emsp;cleanTime|清扫任务时间|integer||
|&emsp;&emsp;&emsp;&emsp;companyName|公司名称|string||
|&emsp;&emsp;&emsp;&emsp;componentIdList||string||
|&emsp;&emsp;&emsp;&emsp;createTime|创建时间|string||
|&emsp;&emsp;&emsp;&emsp;deviceRosVersion|core版本|string||
|&emsp;&emsp;&emsp;&emsp;disinfectStatus|消杀状态|integer||
|&emsp;&emsp;&emsp;&emsp;endTime|执行结束时间|string||
|&emsp;&emsp;&emsp;&emsp;errMsg||string||
|&emsp;&emsp;&emsp;&emsp;errorCode|错误代码|string||
|&emsp;&emsp;&emsp;&emsp;errorMessage|异常信息|string||
|&emsp;&emsp;&emsp;&emsp;executeTime|执行开始时间|string||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;iotId|设备id|string||
|&emsp;&emsp;&emsp;&emsp;isComplete||integer||
|&emsp;&emsp;&emsp;&emsp;isSuccess|是否成功|integer||
|&emsp;&emsp;&emsp;&emsp;mapId||string||
|&emsp;&emsp;&emsp;&emsp;mapName|地图名称|string||
|&emsp;&emsp;&emsp;&emsp;mopStatus|湿拖等级|integer||
|&emsp;&emsp;&emsp;&emsp;name|设备名称|string||
|&emsp;&emsp;&emsp;&emsp;nickname|设备名称|string||
|&emsp;&emsp;&emsp;&emsp;operEvent|手动事件|string||
|&emsp;&emsp;&emsp;&emsp;planId|planId|integer||
|&emsp;&emsp;&emsp;&emsp;projectId|项目ID|string||
|&emsp;&emsp;&emsp;&emsp;projectName|项目名称|string||
|&emsp;&emsp;&emsp;&emsp;pushStatus|尘推等级|integer||
|&emsp;&emsp;&emsp;&emsp;rate|执行次数|integer||
|&emsp;&emsp;&emsp;&emsp;rechargeRetries|返回基站次数|integer||
|&emsp;&emsp;&emsp;&emsp;skipPathPointCount||integer||
|&emsp;&emsp;&emsp;&emsp;snapshotId||string||
|&emsp;&emsp;&emsp;&emsp;startMode|任务类型|string||
|&emsp;&emsp;&emsp;&emsp;status|待上传=0 上传中=1 已上传=2 已失效=3 上传失败=4|integer||
|&emsp;&emsp;&emsp;&emsp;sweepStatus|扫头状态|integer||
|&emsp;&emsp;&emsp;&emsp;taskId|任务id|string||
|&emsp;&emsp;&emsp;&emsp;taskMode|任务模式|integer||
|&emsp;&emsp;&emsp;&emsp;taskName|任务名称|string||
|&emsp;&emsp;&emsp;&emsp;taskType|启动方式|integer||
|&emsp;&emsp;&emsp;&emsp;totalPathPointCount||integer||
|&emsp;&emsp;&emsp;&emsp;type|类型|string||
|&emsp;&emsp;&emsp;&emsp;updateTime|更新时间|string||
|&emsp;&emsp;&emsp;&emsp;vacuumStatus|风机等级|integer||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"aromatherapyStatus": 0,
				"baseComplete": 0,
				"cleanArea": 0,
				"cleanTime": 0,
				"companyName": "",
				"componentIdList": "",
				"createTime": "",
				"deviceRosVersion": "",
				"disinfectStatus": 0,
				"endTime": "",
				"errMsg": "",
				"errorCode": "",
				"errorMessage": "",
				"executeTime": "",
				"id": "",
				"iotId": "",
				"isComplete": 0,
				"isSuccess": 0,
				"mapId": "",
				"mapName": "",
				"mopStatus": 0,
				"name": "",
				"nickname": "",
				"operEvent": "",
				"planId": 0,
				"projectId": "",
				"projectName": "",
				"pushStatus": 0,
				"rate": 0,
				"rechargeRetries": 0,
				"skipPathPointCount": 0,
				"snapshotId": "",
				"startMode": "",
				"status": 0,
				"sweepStatus": 0,
				"taskId": "",
				"taskMode": 0,
				"taskName": "",
				"taskType": 0,
				"totalPathPointCount": 0,
				"type": "",
				"updateTime": "",
				"vacuumStatus": 0
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 清洁记录报表导出


**接口地址**:`/nebula/cleantask/uncompleted-export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>清洁记录报表导出</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|core|软件版本|query|false|string||
|deviceName|设备编号|query|false|string||
|endDate|结束日期|query|false|string||
|isSuccess|状态|query|false|integer(int32)||
|nickName|设备名|query|false|string||
|project|项目|query|false|string||
|startDate|开始日期|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 清洁记录报表


**接口地址**:`/nebula/cleantask/uncompleted-page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>清洁记录报表分页</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|core|软件版本|query|false|string||
|deviceName|设备编号|query|false|string||
|endDate|结束日期|query|false|string||
|isSuccess|状态|query|false|integer(int32)||
|nickName|设备名|query|false|string||
|project|项目|query|false|string||
|startDate|开始日期|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«CleanTask对象»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«CleanTask对象»|IPage«CleanTask对象»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|CleanTask对象|
|&emsp;&emsp;&emsp;&emsp;aromatherapyStatus|香薰状态|integer||
|&emsp;&emsp;&emsp;&emsp;baseComplete||integer||
|&emsp;&emsp;&emsp;&emsp;cleanArea|清扫面积|integer||
|&emsp;&emsp;&emsp;&emsp;cleanTime|清扫任务时间|integer||
|&emsp;&emsp;&emsp;&emsp;companyName|公司名称|string||
|&emsp;&emsp;&emsp;&emsp;componentIdList||string||
|&emsp;&emsp;&emsp;&emsp;createTime|创建时间|string||
|&emsp;&emsp;&emsp;&emsp;deviceRosVersion|core版本|string||
|&emsp;&emsp;&emsp;&emsp;disinfectStatus|消杀状态|integer||
|&emsp;&emsp;&emsp;&emsp;endTime|执行结束时间|string||
|&emsp;&emsp;&emsp;&emsp;errMsg||string||
|&emsp;&emsp;&emsp;&emsp;errorCode|错误代码|string||
|&emsp;&emsp;&emsp;&emsp;errorMessage|异常信息|string||
|&emsp;&emsp;&emsp;&emsp;executeTime|执行开始时间|string||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;iotId|设备id|string||
|&emsp;&emsp;&emsp;&emsp;isComplete||integer||
|&emsp;&emsp;&emsp;&emsp;isSuccess|是否成功|integer||
|&emsp;&emsp;&emsp;&emsp;mapId||string||
|&emsp;&emsp;&emsp;&emsp;mapName|地图名称|string||
|&emsp;&emsp;&emsp;&emsp;mopStatus|湿拖等级|integer||
|&emsp;&emsp;&emsp;&emsp;name|设备名称|string||
|&emsp;&emsp;&emsp;&emsp;nickname|设备名称|string||
|&emsp;&emsp;&emsp;&emsp;operEvent|手动事件|string||
|&emsp;&emsp;&emsp;&emsp;planId|planId|integer||
|&emsp;&emsp;&emsp;&emsp;projectId|项目ID|string||
|&emsp;&emsp;&emsp;&emsp;projectName|项目名称|string||
|&emsp;&emsp;&emsp;&emsp;pushStatus|尘推等级|integer||
|&emsp;&emsp;&emsp;&emsp;rate|执行次数|integer||
|&emsp;&emsp;&emsp;&emsp;rechargeRetries|返回基站次数|integer||
|&emsp;&emsp;&emsp;&emsp;skipPathPointCount||integer||
|&emsp;&emsp;&emsp;&emsp;snapshotId||string||
|&emsp;&emsp;&emsp;&emsp;startMode|任务类型|string||
|&emsp;&emsp;&emsp;&emsp;status|待上传=0 上传中=1 已上传=2 已失效=3 上传失败=4|integer||
|&emsp;&emsp;&emsp;&emsp;sweepStatus|扫头状态|integer||
|&emsp;&emsp;&emsp;&emsp;taskId|任务id|string||
|&emsp;&emsp;&emsp;&emsp;taskMode|任务模式|integer||
|&emsp;&emsp;&emsp;&emsp;taskName|任务名称|string||
|&emsp;&emsp;&emsp;&emsp;taskType|启动方式|integer||
|&emsp;&emsp;&emsp;&emsp;totalPathPointCount||integer||
|&emsp;&emsp;&emsp;&emsp;type|类型|string||
|&emsp;&emsp;&emsp;&emsp;updateTime|更新时间|string||
|&emsp;&emsp;&emsp;&emsp;vacuumStatus|风机等级|integer||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"aromatherapyStatus": 0,
				"baseComplete": 0,
				"cleanArea": 0,
				"cleanTime": 0,
				"companyName": "",
				"componentIdList": "",
				"createTime": "",
				"deviceRosVersion": "",
				"disinfectStatus": 0,
				"endTime": "",
				"errMsg": "",
				"errorCode": "",
				"errorMessage": "",
				"executeTime": "",
				"id": "",
				"iotId": "",
				"isComplete": 0,
				"isSuccess": 0,
				"mapId": "",
				"mapName": "",
				"mopStatus": 0,
				"name": "",
				"nickname": "",
				"operEvent": "",
				"planId": 0,
				"projectId": "",
				"projectName": "",
				"pushStatus": 0,
				"rate": 0,
				"rechargeRetries": 0,
				"skipPathPointCount": 0,
				"snapshotId": "",
				"startMode": "",
				"status": 0,
				"sweepStatus": 0,
				"taskId": "",
				"taskMode": 0,
				"taskName": "",
				"taskType": 0,
				"totalPathPointCount": 0,
				"type": "",
				"updateTime": "",
				"vacuumStatus": 0
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


# 清洁任务失败原因配置


## 新增失败原因


**接口地址**:`/nebula/cleantask/fail/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>新增失败原因</p>



**请求示例**:


```javascript
{
  "failName": "",
  "id": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|CleanTaskFailConfigDto|CleanTaskFailConfigDto|
|&emsp;&emsp;failName|失败原因||false|string||
|&emsp;&emsp;id|id||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 通过id删除


**接口地址**:`/nebula/cleantask/fail/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>通过id删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 查询所有


**接口地址**:`/nebula/cleantask/fail/getAll`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询所有</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 通过id修改


**接口地址**:`/nebula/cleantask/fail/update`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>通过id修改</p>



**请求示例**:


```javascript
{
  "failName": "",
  "id": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|CleanTaskFailConfigDto|CleanTaskFailConfigDto|
|&emsp;&emsp;failName|失败原因||false|string||
|&emsp;&emsp;id|id||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 清洁记录事件码


## 新增


**接口地址**:`/nebula/cleantask/config/create`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>新增</p>



**请求示例**:


```javascript
{
  "code": "",
  "content": "",
  "createTime": "",
  "id": "",
  "remake": "",
  "type": "",
  "updateTime": "",
  "usable": true
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|cleanTaskConfigEntity|清扫记录时间定义表|body|true|CleanTaskConfig对象|CleanTaskConfig对象|
|&emsp;&emsp;code|事件码||true|string||
|&emsp;&emsp;content|WEB展示内容||true|string||
|&emsp;&emsp;createTime|||false|string(date-time)||
|&emsp;&emsp;id|||false|string||
|&emsp;&emsp;remake|事件描述||true|string||
|&emsp;&emsp;type|事件类型||true|string||
|&emsp;&emsp;updateTime|||false|string(date-time)||
|&emsp;&emsp;usable|||false|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除


**接口地址**:`/nebula/cleantask/config/del`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 修改


**接口地址**:`/nebula/cleantask/config/edit`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>修改</p>



**请求示例**:


```javascript
{
  "code": "",
  "content": "",
  "createTime": "",
  "id": "",
  "remake": "",
  "type": "",
  "updateTime": "",
  "usable": true
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|cleanTaskConfigEntity|清扫记录时间定义表|body|true|CleanTaskConfig对象|CleanTaskConfig对象|
|&emsp;&emsp;code|事件码||true|string||
|&emsp;&emsp;content|WEB展示内容||true|string||
|&emsp;&emsp;createTime|||false|string(date-time)||
|&emsp;&emsp;id|||false|string||
|&emsp;&emsp;remake|事件描述||true|string||
|&emsp;&emsp;type|事件类型||true|string||
|&emsp;&emsp;updateTime|||false|string(date-time)||
|&emsp;&emsp;usable|||false|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取事件码


**接口地址**:`/nebula/cleantask/config/event-code`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取事件码</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«string»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [],
	"success": true,
	"timestamp": 0
}
```


## 导出


**接口地址**:`/nebula/cleantask/config/export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>导出</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|code|中断码|query|false|string||
|endDate|结束时间|query|false|string||
|startDate|开始时间|query|false|string||
|type|中断类型|query|false|string||
|usable|是否生效|query|false|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 导入


**接口地址**:`/nebula/cleantask/config/import`


**请求方式**:`POST`


**请求数据类型**:`multipart/form-data`


**响应数据类型**:`*/*`


**接口描述**:<p>导入</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|file|file|body|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 分页


**接口地址**:`/nebula/cleantask/config/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|code|中断码|query|false|string||
|endDate|结束时间|query|false|string||
|startDate|开始时间|query|false|string||
|type|中断类型|query|false|string||
|usable|是否生效|query|false|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«CleanTaskConfig对象»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«CleanTaskConfig对象»|IPage«CleanTaskConfig对象»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|CleanTaskConfig对象|
|&emsp;&emsp;&emsp;&emsp;code|事件码|string||
|&emsp;&emsp;&emsp;&emsp;content|WEB展示内容|string||
|&emsp;&emsp;&emsp;&emsp;createTime||string||
|&emsp;&emsp;&emsp;&emsp;id||string||
|&emsp;&emsp;&emsp;&emsp;remake|事件描述|string||
|&emsp;&emsp;&emsp;&emsp;type|事件类型|string||
|&emsp;&emsp;&emsp;&emsp;updateTime||string||
|&emsp;&emsp;&emsp;&emsp;usable||boolean||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"code": "",
				"content": "",
				"createTime": "",
				"id": "",
				"remake": "",
				"type": "",
				"updateTime": "",
				"usable": true
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 下载模板


**接口地址**:`/nebula/cleantask/config/template`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>下载模板</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 获取中断类型


**接口地址**:`/nebula/cleantask/config/types`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取中断类型</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 用户登录


## 登录二维码


**接口地址**:`/nebula/sys/getLoginQrcode`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>登录二维码</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取用户扫码后保存的token


**接口地址**:`/nebula/sys/getQrcodeToken`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取用户扫码后保存的token</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|qrcodeId|qrcodeId|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 登录接口


**接口地址**:`/nebula/sys/login`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:


**请求示例**:


```javascript
{
  "captcha": "",
  "checkKey": "",
  "password": "",
  "username": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysLoginModel|登录对象|body|true|登录对象|登录对象|
|&emsp;&emsp;captcha|验证码||false|string||
|&emsp;&emsp;checkKey|验证码key||false|string||
|&emsp;&emsp;password|密码||false|string||
|&emsp;&emsp;username|账号||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«JSONObject»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 手机号登录接口


**接口地址**:`/nebula/sys/phoneLogin`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|jsonObject|jsonObject|body|true|||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«JSONObject»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取验证码


**接口地址**:`/nebula/sys/randomImage/{key}`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|key|key|path|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«string»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": "",
	"success": true,
	"timestamp": 0
}
```


## 扫码登录二维码


**接口地址**:`/nebula/sys/scanLoginQrcode`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>扫码登录二维码</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|qrcodeId|qrcodeId|query|true|string||
|token|token|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取小程序用户信息


**接口地址**:`/nebula/sys/wx/user`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|code|code|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 小程序登录接口


**接口地址**:`/nebula/sys/wxPhone`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|code|code|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 电梯控制


## 呼叫电梯


**接口地址**:`/nebula/lift/call`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>呼叫电梯</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|liftNo|电梯编号|query|true|integer(int32)||
|placeId|地点id|query|true|string||
|toFloor|目标楼层|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 预约-取消


**接口地址**:`/nebula/lift/lock`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>预约/取消</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|fromFloor|起始楼层|query|true|integer(int32)||
|liftNo|电梯编号|query|true|integer(int32)||
|locked|预约操作。0 取消预约，1 预约|query|true|integer(int32)||
|placeId|地点id|query|true|string||
|toFloor|目标楼层|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 延迟开-关门接口


**接口地址**:`/nebula/lift/open`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>延迟开/关门接口</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|liftNo|电梯编号|query|true|string||
|placeId|地点id|query|true|string||
|seconds|延迟秒数|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 查询状态


**接口地址**:`/nebula/lift/status`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询状态</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|liftNo|电梯编号|query|true|integer(int32)||
|placeId|地点id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 系统通知


## 系统更新日志


**接口地址**:`/nebula/sys/annountCement/getSystemUpdateLog`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>系统更新日志</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|currentPage| 当前页 |query|true|integer(int64)||
|pageSize| 页面大小 |query|true|integer(int64)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«SysUpdateLogVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array|SysUpdateLogVo|
|&emsp;&emsp;createTime|日志创建时间|string(date-time)||
|&emsp;&emsp;currentPage|当前页|integer(int64)||
|&emsp;&emsp;logContent|日志内容|string||
|&emsp;&emsp;title|日志标题|string||
|&emsp;&emsp;totalPage|总页数|integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [
		{
			"createTime": "",
			"currentPage": 0,
			"logContent": "",
			"title": "",
			"totalPage": 0
		}
	],
	"success": true,
	"timestamp": 0
}
```


# 统计分析


## 小程序数据统计


**接口地址**:`/nebula/statistical/appletStatistical`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>小程序数据统计</p>



**请求示例**:


```javascript
{
  "appletAboutDeviceNameVos": [
    {
      "id": "",
      "type": ""
    }
  ],
  "day": 0
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|appletStatisticalDto|appletStatisticalDto|body|false|AppletStatisticalDto|AppletStatisticalDto|
|&emsp;&emsp;appletAboutDeviceNameVos|||false|array|AppletAboutDeviceNameVo|
|&emsp;&emsp;&emsp;&emsp;id|id 或者 deviceName||false|string||
|&emsp;&emsp;&emsp;&emsp;type|类型 空 company project||false|string||
|&emsp;&emsp;day|||false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«AppletStatisticalVo»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|AppletStatisticalVo|AppletStatisticalVo|
|&emsp;&emsp;deviceActive||array|StatisticsDataTableVo|
|&emsp;&emsp;&emsp;&emsp;countSize||string||
|&emsp;&emsp;&emsp;&emsp;date||string||
|&emsp;&emsp;deviceCleanArea||array|StatisticsDataTableVo|
|&emsp;&emsp;&emsp;&emsp;countSize||string||
|&emsp;&emsp;&emsp;&emsp;date||string||
|&emsp;&emsp;deviceNumber||integer(int32)||
|&emsp;&emsp;taskTimes||array|StatisticsDataTableVo|
|&emsp;&emsp;&emsp;&emsp;countSize||string||
|&emsp;&emsp;&emsp;&emsp;date||string||
|&emsp;&emsp;workTime||array|StatisticsDataTableVo|
|&emsp;&emsp;&emsp;&emsp;countSize||string||
|&emsp;&emsp;&emsp;&emsp;date||string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"deviceActive": [
			{
				"countSize": "",
				"date": ""
			}
		],
		"deviceCleanArea": [
			{
				"countSize": "",
				"date": ""
			}
		],
		"deviceNumber": 0,
		"taskTimes": [
			{
				"countSize": "",
				"date": ""
			}
		],
		"workTime": [
			{
				"countSize": "",
				"date": ""
			}
		]
	},
	"success": true,
	"timestamp": 0
}
```


## 单设备清洁记录统计


**接口地址**:`/nebula/statistical/clean-record-statistics`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>单设备清洁记录统计</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 覆盖率统计


**接口地址**:`/nebula/statistical/coverageRate`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>覆盖率统计</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|menu|1 2 |query|true|integer(int32)||
|date|目前日期参数暂不使用|query|false|string||
|exportForm| ASC 升序 DESC 降序|query|false|string||
|iotId|单设备目前不支持|query|false|string||
|project|menu=2 时传入项目id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«TaskCoverageRateVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|TaskCoverageRateVo|TaskCoverageRateVo|
|&emsp;&emsp;coverageRate||array|integer|
|&emsp;&emsp;nameList||array|string|
|&emsp;&emsp;taskArea||array|number|
|&emsp;&emsp;totalArea||array|number|
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"coverageRate": [],
		"nameList": [],
		"taskArea": [],
		"totalArea": []
	},
	"success": true,
	"timestamp": 0
}
```


## 覆盖率统计导出


**接口地址**:`/nebula/statistical/coverageRate/export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>覆盖率统计导出</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|menu|1 2 |query|true|integer(int32)||
|date|目前功能无需传入|query|false|string||
|exportForm|导出传入 ASC 升序 DESC 降序|query|false|string||
|iotId|目前功能无需传入|query|false|string||
|project|menu=2 时传入项目id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 清洁面积统计


**接口地址**:`/nebula/statistical/data/device/clean`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>清洁面积统计</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|coreVersion|core版本|query|false|string||
|device|设备编号|query|false|string||
|endDate|结束时间|query|false|string||
|project|项目id|query|false|string||
|startDate|开始时间|query|false|string||
|taskStartType|任务启动方式|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 设备告警统计


**接口地址**:`/nebula/statistical/data/device/task/alarm`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>设备告警统计</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|coreVersion|core版本|query|false|string||
|device|设备编号|query|false|string||
|endDate|结束时间|query|false|string||
|project|项目id|query|false|string||
|startDate|开始时间|query|false|string||
|taskStartType|任务启动方式|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 设备中断原因


**接口地址**:`/nebula/statistical/data/device/task/fail`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>设备中断原因</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|coreVersion|core版本|query|false|string||
|device|设备编号|query|false|string||
|endDate|结束时间|query|false|string||
|project|项目id|query|false|string||
|startDate|开始时间|query|false|string||
|taskStartType|任务启动方式|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 设备任务tsr


**接口地址**:`/nebula/statistical/data/device/task/tsr`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>设备任务tsr</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|coreVersion|core版本|query|false|string||
|device|设备编号|query|false|string||
|endDate|结束时间|query|false|string||
|project|项目id|query|false|string||
|startDate|开始时间|query|false|string||
|taskStartType|任务启动方式|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 数据总览


**接口地址**:`/nebula/statistical/data/query`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>数据总览</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|coreVersion|core版本|query|false|string||
|device|设备编号|query|false|string||
|endDate|结束时间|query|false|string||
|project|项目id|query|false|string||
|startDate|开始时间|query|false|string||
|taskStartType|任务启动方式|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 异常告警统计


**接口地址**:`/nebula/statistical/data/task/alarm`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>异常告警统计</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|coreVersion|core版本|query|false|string||
|device|设备编号|query|false|string||
|endDate|结束时间|query|false|string||
|project|项目id|query|false|string||
|startDate|开始时间|query|false|string||
|taskStartType|任务启动方式|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 任务失败统计


**接口地址**:`/nebula/statistical/data/task/fail`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>任务失败统计</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|coreVersion|core版本|query|false|string||
|device|设备编号|query|false|string||
|endDate|结束时间|query|false|string||
|project|项目id|query|false|string||
|startDate|开始时间|query|false|string||
|taskStartType|任务启动方式|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 任务失败原因分析


**接口地址**:`/nebula/statistical/data/task/fail/reason`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>任务失败原因分析</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|coreVersion|core版本|query|false|string||
|device|设备编号|query|false|string||
|endDate|结束时间|query|false|string||
|project|项目id|query|false|string||
|startDate|开始时间|query|false|string||
|taskStartType|任务启动方式|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 设备活跃度


**接口地址**:`/nebula/statistical/device/activation`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>设备活跃度</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|endDate|活跃度统计结束时间|query|false|string||
|project|所属项目(多个项目用逗号分隔)|query|false|string||
|startDate|活跃度统计的开始时间day格式(xxxx-xx-xx) week(xxxx-xx-xx)|query|false|string||
|type|week 周活 day 日活|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«ActivationVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|ActivationVo|ActivationVo|
|&emsp;&emsp;activationSizes||array|integer|
|&emsp;&emsp;activations||array|number|
|&emsp;&emsp;dates||array|string|
|&emsp;&emsp;deviceSizes||integer(int64)||
|&emsp;&emsp;name||string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"activationSizes": [],
		"activations": [],
		"dates": [],
		"deviceSizes": 0,
		"name": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 数据报表


**接口地址**:`/nebula/statistical/device/data/table`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>数据报表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|isHome|是否为一级菜单显示 0否  1是|query|true|integer(int32)||
|menu|所属菜单 1设备在线数,2工作时长,3平均工作时长|query|true|integer(int32)||
|project|所属项目|query|false|string||
|type|month月,week周,day天|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 数据报表-导出


**接口地址**:`/nebula/statistical/device/data/table/export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>数据报表-导出</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|isHome|isHome|query|true|integer(int32)||
|menu|menu|query|true|integer(int32)||
|form|device 设备 （默认不填 新增方式）|query|false|string||
|project|project|query|false|string||
|type|month月,week周,day天|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 统计分析首页


**接口地址**:`/nebula/statistical/device/data/table/home`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>统计分析首页</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 作业统计


**接口地址**:`/nebula/statistical/history/week/statistics`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>作业统计</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|date|date|query|true|string||
|iotId|iotId|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 作业统计导出


**接口地址**:`/nebula/statistical/history/week/statistics/export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>作业统计导出</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|date|date|query|true|string||
|iotId|iotId|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 首页设备统计


**接口地址**:`/nebula/statistical/home`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>首页设备统计</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## tsr成功率


**接口地址**:`/nebula/statistical/tsr/success-rate`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>tsr成功率</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|date|日期，字符串类型，如2022-07-07|query|true|string||
|project|项目名|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## tsr成功率详情


**接口地址**:`/nebula/statistical/tsr/success-rate-detail`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>tsr成功率详情</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|date|日期，字符串类型，如2022-07-07|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|project|项目名|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## tsr成功率详情导出


**接口地址**:`/nebula/statistical/tsr/success-rate-detail-export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>tsr成功率详情导出</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|date|日期，字符串类型，如2022-07-07|query|true|string||
|project|项目名|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


# 编码校验规则


## 编码校验规则-添加


**接口地址**:`/nebula/sys/checkRule/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>编码校验规则-添加</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "id": "",
  "ruleCode": "",
  "ruleDescription": "",
  "ruleJson": "",
  "ruleName": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysCheckRule|编码校验规则|body|true|sys_check_rule对象|sys_check_rule对象|
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;id|主键id||false|string||
|&emsp;&emsp;ruleCode|规则Code||false|string||
|&emsp;&emsp;ruleDescription|规则描述||false|string||
|&emsp;&emsp;ruleJson|规则JSON||false|string||
|&emsp;&emsp;ruleName|规则名称||false|string||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 编码校验规则-通过Code校验传入的值


**接口地址**:`/nebula/sys/checkRule/checkByCode`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>编码校验规则-通过Code校验传入的值</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|ruleCode|ruleCode|query|true|string||
|value|value|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 编码校验规则-通过id删除


**接口地址**:`/nebula/sys/checkRule/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>编码校验规则-通过id删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 编码校验规则-批量删除


**接口地址**:`/nebula/sys/checkRule/deleteBatch`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>编码校验规则-批量删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|ids|ids|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 编码校验规则-编辑


**接口地址**:`/nebula/sys/checkRule/edit`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>编码校验规则-编辑</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "id": "",
  "ruleCode": "",
  "ruleDescription": "",
  "ruleJson": "",
  "ruleName": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysCheckRule|编码校验规则|body|true|sys_check_rule对象|sys_check_rule对象|
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;id|主键id||false|string||
|&emsp;&emsp;ruleCode|规则Code||false|string||
|&emsp;&emsp;ruleDescription|规则描述||false|string||
|&emsp;&emsp;ruleJson|规则JSON||false|string||
|&emsp;&emsp;ruleName|规则名称||false|string||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 编码校验规则-编辑


**接口地址**:`/nebula/sys/checkRule/edit`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>编码校验规则-编辑</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "id": "",
  "ruleCode": "",
  "ruleDescription": "",
  "ruleJson": "",
  "ruleName": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysCheckRule|编码校验规则|body|true|sys_check_rule对象|sys_check_rule对象|
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;id|主键id||false|string||
|&emsp;&emsp;ruleCode|规则Code||false|string||
|&emsp;&emsp;ruleDescription|规则描述||false|string||
|&emsp;&emsp;ruleJson|规则JSON||false|string||
|&emsp;&emsp;ruleName|规则名称||false|string||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 编码校验规则-分页列表查询


**接口地址**:`/nebula/sys/checkRule/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>编码校验规则-分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|createBy|创建人|query|false|string||
|createTime|创建时间|query|false|string(date-time)||
|id|主键id|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|ruleCode|规则Code|query|false|string||
|ruleDescription|规则描述|query|false|string||
|ruleJson|规则JSON|query|false|string||
|ruleName|规则名称|query|false|string||
|updateBy|更新人|query|false|string||
|updateTime|更新时间|query|false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 编码校验规则-通过id查询


**接口地址**:`/nebula/sys/checkRule/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>编码校验规则-通过id查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 职务表


## 职务表-添加


**接口地址**:`/nebula/sys/position/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>职务表-添加</p>



**请求示例**:


```javascript
{
  "code": "",
  "companyId": "",
  "createBy": "",
  "createTime": "",
  "id": "",
  "name": "",
  "postRank": "",
  "sysOrgCode": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysPosition|职务表|body|true|sys_position对象|sys_position对象|
|&emsp;&emsp;code|职务编码||false|string||
|&emsp;&emsp;companyId|公司id||false|string||
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;name|职务名称||false|string||
|&emsp;&emsp;postRank|职级||false|string||
|&emsp;&emsp;sysOrgCode|组织机构编码||false|string||
|&emsp;&emsp;updateBy|修改人||false|string||
|&emsp;&emsp;updateTime|修改时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«sys_position对象»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|sys_position对象|sys_position对象|
|&emsp;&emsp;code|职务编码|string||
|&emsp;&emsp;companyId|公司id|string||
|&emsp;&emsp;createBy|创建人|string||
|&emsp;&emsp;createTime|创建时间|string(date-time)||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;name|职务名称|string||
|&emsp;&emsp;postRank|职级|string||
|&emsp;&emsp;sysOrgCode|组织机构编码|string||
|&emsp;&emsp;updateBy|修改人|string||
|&emsp;&emsp;updateTime|修改时间|string(date-time)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"code": "",
		"companyId": "",
		"createBy": "",
		"createTime": "",
		"id": "",
		"name": "",
		"postRank": "",
		"sysOrgCode": "",
		"updateBy": "",
		"updateTime": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 职务表-通过id删除


**接口地址**:`/nebula/sys/position/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>职务表-通过id删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 职务表-批量删除


**接口地址**:`/nebula/sys/position/deleteBatch`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>职务表-批量删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|ids|ids|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«sys_position对象»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|sys_position对象|sys_position对象|
|&emsp;&emsp;code|职务编码|string||
|&emsp;&emsp;companyId|公司id|string||
|&emsp;&emsp;createBy|创建人|string||
|&emsp;&emsp;createTime|创建时间|string(date-time)||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;name|职务名称|string||
|&emsp;&emsp;postRank|职级|string||
|&emsp;&emsp;sysOrgCode|组织机构编码|string||
|&emsp;&emsp;updateBy|修改人|string||
|&emsp;&emsp;updateTime|修改时间|string(date-time)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"code": "",
		"companyId": "",
		"createBy": "",
		"createTime": "",
		"id": "",
		"name": "",
		"postRank": "",
		"sysOrgCode": "",
		"updateBy": "",
		"updateTime": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 职务表-编辑


**接口地址**:`/nebula/sys/position/edit`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>职务表-编辑</p>



**请求示例**:


```javascript
{
  "code": "",
  "companyId": "",
  "createBy": "",
  "createTime": "",
  "id": "",
  "name": "",
  "postRank": "",
  "sysOrgCode": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysPosition|职务表|body|true|sys_position对象|sys_position对象|
|&emsp;&emsp;code|职务编码||false|string||
|&emsp;&emsp;companyId|公司id||false|string||
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;name|职务名称||false|string||
|&emsp;&emsp;postRank|职级||false|string||
|&emsp;&emsp;sysOrgCode|组织机构编码||false|string||
|&emsp;&emsp;updateBy|修改人||false|string||
|&emsp;&emsp;updateTime|修改时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«sys_position对象»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|sys_position对象|sys_position对象|
|&emsp;&emsp;code|职务编码|string||
|&emsp;&emsp;companyId|公司id|string||
|&emsp;&emsp;createBy|创建人|string||
|&emsp;&emsp;createTime|创建时间|string(date-time)||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;name|职务名称|string||
|&emsp;&emsp;postRank|职级|string||
|&emsp;&emsp;sysOrgCode|组织机构编码|string||
|&emsp;&emsp;updateBy|修改人|string||
|&emsp;&emsp;updateTime|修改时间|string(date-time)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"code": "",
		"companyId": "",
		"createBy": "",
		"createTime": "",
		"id": "",
		"name": "",
		"postRank": "",
		"sysOrgCode": "",
		"updateBy": "",
		"updateTime": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 职务表-编辑


**接口地址**:`/nebula/sys/position/edit`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>职务表-编辑</p>



**请求示例**:


```javascript
{
  "code": "",
  "companyId": "",
  "createBy": "",
  "createTime": "",
  "id": "",
  "name": "",
  "postRank": "",
  "sysOrgCode": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysPosition|职务表|body|true|sys_position对象|sys_position对象|
|&emsp;&emsp;code|职务编码||false|string||
|&emsp;&emsp;companyId|公司id||false|string||
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;name|职务名称||false|string||
|&emsp;&emsp;postRank|职级||false|string||
|&emsp;&emsp;sysOrgCode|组织机构编码||false|string||
|&emsp;&emsp;updateBy|修改人||false|string||
|&emsp;&emsp;updateTime|修改时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«sys_position对象»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|sys_position对象|sys_position对象|
|&emsp;&emsp;code|职务编码|string||
|&emsp;&emsp;companyId|公司id|string||
|&emsp;&emsp;createBy|创建人|string||
|&emsp;&emsp;createTime|创建时间|string(date-time)||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;name|职务名称|string||
|&emsp;&emsp;postRank|职级|string||
|&emsp;&emsp;sysOrgCode|组织机构编码|string||
|&emsp;&emsp;updateBy|修改人|string||
|&emsp;&emsp;updateTime|修改时间|string(date-time)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"code": "",
		"companyId": "",
		"createBy": "",
		"createTime": "",
		"id": "",
		"name": "",
		"postRank": "",
		"sysOrgCode": "",
		"updateBy": "",
		"updateTime": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 职务表-分页列表查询


**接口地址**:`/nebula/sys/position/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>职务表-分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|code|职务编码|query|false|string||
|companyId|公司id|query|false|string||
|createBy|创建人|query|false|string||
|createTime|创建时间|query|false|string(date-time)||
|id|id|query|false|string||
|name|职务名称|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|postRank|职级|query|false|string||
|sysOrgCode|组织机构编码|query|false|string||
|updateBy|修改人|query|false|string||
|updateTime|修改时间|query|false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«sys_position对象»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«sys_position对象»|IPage«sys_position对象»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|sys_position对象|
|&emsp;&emsp;&emsp;&emsp;code|职务编码|string||
|&emsp;&emsp;&emsp;&emsp;companyId|公司id|string||
|&emsp;&emsp;&emsp;&emsp;createBy|创建人|string||
|&emsp;&emsp;&emsp;&emsp;createTime|创建时间|string||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;name|职务名称|string||
|&emsp;&emsp;&emsp;&emsp;postRank|职级|string||
|&emsp;&emsp;&emsp;&emsp;sysOrgCode|组织机构编码|string||
|&emsp;&emsp;&emsp;&emsp;updateBy|修改人|string||
|&emsp;&emsp;&emsp;&emsp;updateTime|修改时间|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"code": "",
				"companyId": "",
				"createBy": "",
				"createTime": "",
				"id": "",
				"name": "",
				"postRank": "",
				"sysOrgCode": "",
				"updateBy": "",
				"updateTime": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 职务表-通过code查询


**接口地址**:`/nebula/sys/position/queryByCode`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>职务表-通过code查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|code|code|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«sys_position对象»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|sys_position对象|sys_position对象|
|&emsp;&emsp;code|职务编码|string||
|&emsp;&emsp;companyId|公司id|string||
|&emsp;&emsp;createBy|创建人|string||
|&emsp;&emsp;createTime|创建时间|string(date-time)||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;name|职务名称|string||
|&emsp;&emsp;postRank|职级|string||
|&emsp;&emsp;sysOrgCode|组织机构编码|string||
|&emsp;&emsp;updateBy|修改人|string||
|&emsp;&emsp;updateTime|修改时间|string(date-time)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"code": "",
		"companyId": "",
		"createBy": "",
		"createTime": "",
		"id": "",
		"name": "",
		"postRank": "",
		"sysOrgCode": "",
		"updateBy": "",
		"updateTime": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 职务表-通过id查询


**接口地址**:`/nebula/sys/position/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>职务表-通过id查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«sys_position对象»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|sys_position对象|sys_position对象|
|&emsp;&emsp;code|职务编码|string||
|&emsp;&emsp;companyId|公司id|string||
|&emsp;&emsp;createBy|创建人|string||
|&emsp;&emsp;createTime|创建时间|string(date-time)||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;name|职务名称|string||
|&emsp;&emsp;postRank|职级|string||
|&emsp;&emsp;sysOrgCode|组织机构编码|string||
|&emsp;&emsp;updateBy|修改人|string||
|&emsp;&emsp;updateTime|修改时间|string(date-time)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"code": "",
		"companyId": "",
		"createBy": "",
		"createTime": "",
		"id": "",
		"name": "",
		"postRank": "",
		"sysOrgCode": "",
		"updateBy": "",
		"updateTime": ""
	},
	"success": true,
	"timestamp": 0
}
```


# 角色首页配置


## 角色首页配置-添加


**接口地址**:`/nebula/sys/sysRoleIndex/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>角色首页配置-添加</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "id": "",
  "priority": 0,
  "roleCode": "",
  "status": "",
  "sysOrgCode": "",
  "updateBy": "",
  "updateTime": "",
  "url": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysRoleIndex|角色首页配置|body|true|sys_role_index对象|sys_role_index对象|
|&emsp;&emsp;createBy|创建人登录名称||false|string||
|&emsp;&emsp;createTime|创建日期||false|string(date-time)||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;priority|优先级||false|integer(int32)||
|&emsp;&emsp;roleCode|角色编码||false|string||
|&emsp;&emsp;status|状态||false|string||
|&emsp;&emsp;sysOrgCode|所属部门||false|string||
|&emsp;&emsp;updateBy|更新人登录名称||false|string||
|&emsp;&emsp;updateTime|更新日期||false|string(date-time)||
|&emsp;&emsp;url|路由地址||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 角色首页配置-通过id删除


**接口地址**:`/nebula/sys/sysRoleIndex/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>角色首页配置-通过id删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 角色首页配置-批量删除


**接口地址**:`/nebula/sys/sysRoleIndex/deleteBatch`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>角色首页配置-批量删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|ids|ids|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 角色首页配置-编辑


**接口地址**:`/nebula/sys/sysRoleIndex/edit`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>角色首页配置-编辑</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "id": "",
  "priority": 0,
  "roleCode": "",
  "status": "",
  "sysOrgCode": "",
  "updateBy": "",
  "updateTime": "",
  "url": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysRoleIndex|角色首页配置|body|true|sys_role_index对象|sys_role_index对象|
|&emsp;&emsp;createBy|创建人登录名称||false|string||
|&emsp;&emsp;createTime|创建日期||false|string(date-time)||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;priority|优先级||false|integer(int32)||
|&emsp;&emsp;roleCode|角色编码||false|string||
|&emsp;&emsp;status|状态||false|string||
|&emsp;&emsp;sysOrgCode|所属部门||false|string||
|&emsp;&emsp;updateBy|更新人登录名称||false|string||
|&emsp;&emsp;updateTime|更新日期||false|string(date-time)||
|&emsp;&emsp;url|路由地址||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 角色首页配置-编辑


**接口地址**:`/nebula/sys/sysRoleIndex/edit`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>角色首页配置-编辑</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "id": "",
  "priority": 0,
  "roleCode": "",
  "status": "",
  "sysOrgCode": "",
  "updateBy": "",
  "updateTime": "",
  "url": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysRoleIndex|角色首页配置|body|true|sys_role_index对象|sys_role_index对象|
|&emsp;&emsp;createBy|创建人登录名称||false|string||
|&emsp;&emsp;createTime|创建日期||false|string(date-time)||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;priority|优先级||false|integer(int32)||
|&emsp;&emsp;roleCode|角色编码||false|string||
|&emsp;&emsp;status|状态||false|string||
|&emsp;&emsp;sysOrgCode|所属部门||false|string||
|&emsp;&emsp;updateBy|更新人登录名称||false|string||
|&emsp;&emsp;updateTime|更新日期||false|string(date-time)||
|&emsp;&emsp;url|路由地址||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 角色首页配置-分页列表查询


**接口地址**:`/nebula/sys/sysRoleIndex/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>角色首页配置-分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|createBy|创建人登录名称|query|false|string||
|createTime|创建日期|query|false|string(date-time)||
|id|id|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|priority|优先级|query|false|integer(int32)||
|roleCode|角色编码|query|false|string||
|status|状态|query|false|string||
|sysOrgCode|所属部门|query|false|string||
|updateBy|更新人登录名称|query|false|string||
|updateTime|更新日期|query|false|string(date-time)||
|url|路由地址|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 角色首页配置-通过code查询


**接口地址**:`/nebula/sys/sysRoleIndex/queryByCode`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>角色首页配置-通过code查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|roleCode|roleCode|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 角色首页配置-通过id查询


**接口地址**:`/nebula/sys/sysRoleIndex/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>角色首页配置-通过id查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 设备异常通知


## 新建错误类型


**接口地址**:`/nebula/alarm/dict/create`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>新建错误类型</p>



**请求示例**:


```javascript
{
  "affectTask": true,
  "alarmLevel": "",
  "errCode": "",
  "errSolution": "",
  "errSym": "",
  "isEnable": true,
  "productKey": "",
  "subModule": "",
  "taskEffect": true
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|createAlarmDictDto|createAlarmDictDto|body|true|CreateAlarmDictDto|CreateAlarmDictDto|
|&emsp;&emsp;affectTask|是否影响任务||false|boolean||
|&emsp;&emsp;alarmLevel|异常等级 P0、P1、P2||false|string||
|&emsp;&emsp;errCode|异常代码||false|string||
|&emsp;&emsp;errSolution|解决建议||false|string||
|&emsp;&emsp;errSym|异常描述||false|string||
|&emsp;&emsp;isEnable|是否启用||false|boolean||
|&emsp;&emsp;productKey|适用型号||false|string||
|&emsp;&emsp;subModule|异常类型||false|string||
|&emsp;&emsp;taskEffect|仅在任务中报警才生效||false|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除消息定义


**接口地址**:`/nebula/alarm/dict/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 导出消息


**接口地址**:`/nebula/alarm/dict/export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>导出消息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|affectTask|是否影响任务|query|false|boolean||
|alarmLevel|异常等级|query|false|string||
|endDate|事件新增结束事件|query|false|string||
|errCode|异常代码|query|false|string||
|isEnable|是否启用|query|false|boolean||
|startDate|事件新增开始事件|query|false|string||
|subModule|异常类型|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 导入消息


**接口地址**:`/nebula/alarm/dict/import`


**请求方式**:`POST`


**请求数据类型**:`multipart/form-data`


**响应数据类型**:`*/*`


**接口描述**:<p>导入消息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|file|file|body|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 消息定义分页


**接口地址**:`/nebula/alarm/dict/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>消息定义分页</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|affectTask|是否影响任务|query|false|boolean||
|alarmLevel|异常等级|query|false|string||
|endDate|事件新增结束事件|query|false|string||
|errCode|异常代码|query|false|string||
|isEnable|是否启用|query|false|boolean||
|startDate|事件新增开始事件|query|false|string||
|subModule|异常类型|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«AlarmDictPageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«AlarmDictPageVo»|IPage«AlarmDictPageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|AlarmDictPageVo|
|&emsp;&emsp;&emsp;&emsp;affectTask|是否影响任务|boolean||
|&emsp;&emsp;&emsp;&emsp;alarmLevel|异常等级|string||
|&emsp;&emsp;&emsp;&emsp;createTime|新增时间|integer||
|&emsp;&emsp;&emsp;&emsp;errCode|异常代码|string||
|&emsp;&emsp;&emsp;&emsp;errSolution|解决建议|string||
|&emsp;&emsp;&emsp;&emsp;errSym|异常描述|string||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;isEnable|是否启用|boolean||
|&emsp;&emsp;&emsp;&emsp;productKey|适用型号|string||
|&emsp;&emsp;&emsp;&emsp;productName|适用型号名称|string||
|&emsp;&emsp;&emsp;&emsp;subModule|异常类型|string||
|&emsp;&emsp;&emsp;&emsp;taskEffect|仅在任务中报警才生效|boolean||
|&emsp;&emsp;&emsp;&emsp;usable|是否可用|boolean||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"affectTask": true,
				"alarmLevel": "",
				"createTime": 0,
				"errCode": "",
				"errSolution": "",
				"errSym": "",
				"id": "",
				"isEnable": true,
				"productKey": "",
				"productName": "",
				"subModule": "",
				"taskEffect": true,
				"usable": true
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 消息订阅列表


**接口地址**:`/nebula/alarm/dict/subscribe/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>消息订阅列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|alarmLevel|异常等级|query|false|string||
|errCode|异常代码|query|false|string||
|subModule|异常类型|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«AlarmSubscribePageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«AlarmSubscribePageVo»|IPage«AlarmSubscribePageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|AlarmSubscribePageVo|
|&emsp;&emsp;&emsp;&emsp;alarmLevel|异常等级|string||
|&emsp;&emsp;&emsp;&emsp;appletsAlarm|是否小程序|boolean||
|&emsp;&emsp;&emsp;&emsp;errCode|异常代码|string||
|&emsp;&emsp;&emsp;&emsp;errSym|异常描述|string||
|&emsp;&emsp;&emsp;&emsp;feishuAlarm|是否飞书告警|boolean||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;operateAlarm|是否运营端告警|boolean||
|&emsp;&emsp;&emsp;&emsp;platformAlarm|是否平台告警|boolean||
|&emsp;&emsp;&emsp;&emsp;subModule|异常类型|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"alarmLevel": "",
				"appletsAlarm": true,
				"errCode": "",
				"errSym": "",
				"feishuAlarm": true,
				"id": "",
				"operateAlarm": true,
				"platformAlarm": true,
				"subModule": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 修改消息订阅


**接口地址**:`/nebula/alarm/dict/subscribe/update`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>修改消息订阅</p>



**请求示例**:


```javascript
[
  {
    "appletsAlarm": true,
    "feishuAlarm": true,
    "id": "",
    "operateAlarm": true,
    "platformAlarm": true
  }
]
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|updateAlarmSubscribeDtos|updateAlarmSubscribeDtos|body|true|array|UpdateAlarmSubscribeDto|
|&emsp;&emsp;appletsAlarm|是否小程序||false|boolean||
|&emsp;&emsp;feishuAlarm|是否飞书告警||false|boolean||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;operateAlarm|是否运营端告警||false|boolean||
|&emsp;&emsp;platformAlarm|是否平台告警||false|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 修改错误类型


**接口地址**:`/nebula/alarm/dict/update`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>修改错误类型</p>



**请求示例**:


```javascript
{
  "affectTask": true,
  "alarmLevel": "",
  "errCode": "",
  "errSolution": "",
  "errSym": "",
  "id": "",
  "isEnable": true,
  "productKey": "",
  "subModule": "",
  "taskEffect": true
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|updateAlarmDictDto|updateAlarmDictDto|body|true|UpdateAlarmDictDto|UpdateAlarmDictDto|
|&emsp;&emsp;affectTask|是否影响任务||false|boolean||
|&emsp;&emsp;alarmLevel|异常等级 P0、P1、P2||false|string||
|&emsp;&emsp;errCode|异常代码||false|string||
|&emsp;&emsp;errSolution|解决建议||false|string||
|&emsp;&emsp;errSym|异常描述||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;isEnable|是否启用||false|boolean||
|&emsp;&emsp;productKey|适用型号||false|string||
|&emsp;&emsp;subModule|异常类型||false|string||
|&emsp;&emsp;taskEffect|仅在任务中报警才生效||false|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取错误类型


**接口地址**:`/nebula/alarm/err-type`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取错误类型</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 分页列表查询


**接口地址**:`/nebula/alarm/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|deviceName|设备名|query|false|string||
|endDate|结束日期|query|false|string||
|eventCode|事件码|query|false|string||
|project|项目id|query|false|string||
|startDate|开始日期|query|false|string||
|type|类型|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«AlarmPageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«AlarmPageVo»|IPage«AlarmPageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|AlarmPageVo|
|&emsp;&emsp;&emsp;&emsp;deviceName|设备名|string||
|&emsp;&emsp;&emsp;&emsp;errName|异常名字|string||
|&emsp;&emsp;&emsp;&emsp;errSolution|故障处理措施|string||
|&emsp;&emsp;&emsp;&emsp;errSym|故障现象|string||
|&emsp;&emsp;&emsp;&emsp;errTime|异常时间|string||
|&emsp;&emsp;&emsp;&emsp;eventCode|事件码|string||
|&emsp;&emsp;&emsp;&emsp;id||integer||
|&emsp;&emsp;&emsp;&emsp;projectName|项目名称|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"deviceName": "",
				"errName": "",
				"errSolution": "",
				"errSym": "",
				"errTime": "",
				"eventCode": "",
				"id": 0,
				"projectName": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 分页列表查询导出


**接口地址**:`/nebula/alarm/list/export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询导出</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|deviceName|设备名|query|false|string||
|endDate|结束日期|query|false|string||
|eventCode|事件码|query|false|string||
|project|项目id|query|false|string||
|startDate|开始日期|query|false|string||
|type|类型|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 任务异常记录分页


**接口地址**:`/nebula/alarm/query/task/alarm/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>任务异常记录分页</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|任务id|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据id查询异常


**接口地址**:`/nebula/alarm/queryByTask`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据id查询异常</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|任务id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«AlarmCleanTaskVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|AlarmCleanTaskVo|AlarmCleanTaskVo|
|&emsp;&emsp;abnormalReason|异常原因|string||
|&emsp;&emsp;alarmEventVos|异常事件|array|AlarmEventVo|
|&emsp;&emsp;&emsp;&emsp;errCode|错误码|string||
|&emsp;&emsp;&emsp;&emsp;errSym|事件|string||
|&emsp;&emsp;&emsp;&emsp;errTime|时间|string||
|&emsp;&emsp;&emsp;&emsp;imageUrl|图片|string||
|&emsp;&emsp;alarmNum|异常条数|integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"abnormalReason": "",
		"alarmEventVos": [
			{
				"errCode": "",
				"errSym": "",
				"errTime": "",
				"imageUrl": ""
			}
		],
		"alarmNum": 0
	},
	"success": true,
	"timestamp": 0
}
```


# 设备操作记录


## 操作历史


**接口地址**:`/nebula/device/operation/detail/history`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>操作历史</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|operationLogId|操作日志id|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«DeviceOnlineLog对象»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«DeviceOnlineLog对象»|IPage«DeviceOnlineLog对象»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|DeviceOnlineLog对象|
|&emsp;&emsp;&emsp;&emsp;createTime|时间|integer||
|&emsp;&emsp;&emsp;&emsp;id||integer||
|&emsp;&emsp;&emsp;&emsp;operationLogId||string||
|&emsp;&emsp;&emsp;&emsp;type|类型 1视频 2远程控制 3任务 4返回基站 5清除电机异常 6重定位 7关机 8暂停任务 9继续任务 10重启|integer||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"createTime": 0,
				"id": 0,
				"operationLogId": "",
				"type": 0
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 保存操作详情


**接口地址**:`/nebula/device/operation/detail/save`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>保存操作详情</p>



**请求示例**:


```javascript
{
  "operationLogId": "",
  "type": 0
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|operationDetailSaveDto|operationDetailSaveDto|body|true|OperationDetailSaveDto|OperationDetailSaveDto|
|&emsp;&emsp;operationLogId|日志id||false|string||
|&emsp;&emsp;type|1视频 2远程控制||false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 分页列表查询


**接口地址**:`/nebula/device/operation/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|deviceName|query|false|string||
|endTime|endTime|query|false|string||
|nickname|nickname|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|projectId|projectId|query|false|string||
|realname|realname|query|false|string||
|startTime|startTime|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«DeviceOperationLogPageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«DeviceOperationLogPageVo»|IPage«DeviceOperationLogPageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|DeviceOperationLogPageVo|
|&emsp;&emsp;&emsp;&emsp;deviceName|设备编号|string||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;nickname|设备名|string||
|&emsp;&emsp;&emsp;&emsp;operateTime|控制开始时间|string||
|&emsp;&emsp;&emsp;&emsp;projectName|项目|string||
|&emsp;&emsp;&emsp;&emsp;realname|操作人|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"deviceName": "",
				"id": "",
				"nickname": "",
				"operateTime": "",
				"projectName": "",
				"realname": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


# 设备文件管理


## 根据id获取地图信息


**接口地址**:`/nebula/file/findById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|id|地图id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取设备地图信息


**接口地址**:`/nebula/file/map`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>包括地图名和下载链接</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名|query|false|string||
|mapId|地图id|query|false|string||
|type|type|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«string»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [],
	"success": true,
	"timestamp": 0
}
```


## 获取设备地图名称列表


**接口地址**:`/nebula/file/map/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备地图名称列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 查询当前设备在用地图信息


**接口地址**:`/nebula/file/map/main`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询当前设备在用地图信息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取设备地图列表


**接口地址**:`/nebula/file/map/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备地图列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|type|type|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取原点


**接口地址**:`/nebula/file/origin`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备ID|query|true|string||
|mapId|地图id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取原始虚拟墙信息


**接口地址**:`/nebula/file/virtual/wall`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取原始虚拟墙信息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备ID|query|true|string||
|mapId|地图id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«MapInfoWallVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array|MapInfoWallVo|
|&emsp;&emsp;point||array|ModelCleanAreaEventPeakDto|
|&emsp;&emsp;&emsp;&emsp;x||number||
|&emsp;&emsp;&emsp;&emsp;y||number||
|&emsp;&emsp;type||integer(int32)||
|&emsp;&emsp;zoneId||integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [
		{
			"point": [
				{
					"x": 0,
					"y": 0
				}
			],
			"type": 0,
			"zoneId": 0
		}
	],
	"success": true,
	"timestamp": 0
}
```


## 获取设备地图的禁区、虚拟墙、清洁区域点位


**接口地址**:`/nebula/file/wall`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备地图的禁区、虚拟墙、清洁区域点位</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备ID|query|true|string||
|mapId|地图id|query|false|string||
|taskId|任务ID|query|false|string||
|taskType|任务类型  0 普通任务  1定时任务|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«MapInfoWallVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array|MapInfoWallVo|
|&emsp;&emsp;point||array|ModelCleanAreaEventPeakDto|
|&emsp;&emsp;&emsp;&emsp;x||number||
|&emsp;&emsp;&emsp;&emsp;y||number||
|&emsp;&emsp;type||integer(int32)||
|&emsp;&emsp;zoneId||integer(int32)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [
		{
			"point": [
				{
					"x": 0,
					"y": 0
				}
			],
			"type": 0,
			"zoneId": 0
		}
	],
	"success": true,
	"timestamp": 0
}
```


# 设备日志模块


## 获取log下载地址


**接口地址**:`/nebula/device/log/file/downLoadLogLink`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取log地址</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|主键|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«FileLinkVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array|FileLinkVo|
|&emsp;&emsp;fileLink|bag链接|string||
|&emsp;&emsp;fileName|bag包名|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [
		{
			"fileLink": "",
			"fileName": ""
		}
	],
	"success": true,
	"timestamp": 0
}
```


## 获取rosBag下载地址


**接口地址**:`/nebula/device/log/file/downLoadRosBagLink`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取rosBag地址</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|taskId|任务Id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List«FileLinkVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array|FileLinkVo|
|&emsp;&emsp;fileLink|bag链接|string||
|&emsp;&emsp;fileName|bag包名|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [
		{
			"fileLink": "",
			"fileName": ""
		}
	],
	"success": true,
	"timestamp": 0
}
```


## 分页


**接口地址**:`/nebula/device/log/file/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|cartoType|carto类型|query|false|string||
|endDate|结束日期|query|false|string||
|fileName|文件名|query|false|string||
|iotId|iotId|query|false|string||
|startDate|开始日期|query|false|string||
|status|状态|query|false|integer(int32)||
|type|文件类型|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«DeviceLogFilePageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«DeviceLogFilePageVo»|IPage«DeviceLogFilePageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|DeviceLogFilePageVo|
|&emsp;&emsp;&emsp;&emsp;cleanTaskId|清洁记录任务id|string||
|&emsp;&emsp;&emsp;&emsp;deviceName|设备名称|string||
|&emsp;&emsp;&emsp;&emsp;fileName|文件名|string||
|&emsp;&emsp;&emsp;&emsp;fileSize|文件大小|string||
|&emsp;&emsp;&emsp;&emsp;id|设备主键|string||
|&emsp;&emsp;&emsp;&emsp;lastUpdateTime|文件最后更新时间|integer||
|&emsp;&emsp;&emsp;&emsp;status|待上传=0 上传中=1 已上传=2 已失效=3|integer||
|&emsp;&emsp;&emsp;&emsp;type|类型 0:app log; 1: carto log;2:nebula log;3:bag;4:app dump|integer||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"cleanTaskId": "",
				"deviceName": "",
				"fileName": "",
				"fileSize": "",
				"id": "",
				"lastUpdateTime": 0,
				"status": 0,
				"type": 0
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 拉取log


**接口地址**:`/nebula/device/log/file/pullLog`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>拉取log</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|false|string||
|id|设备id|query|false|string||
|taskId|任务Id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 设备模块


## 获取设备物模型信息


**接口地址**:`/nebula/device/module/info`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备物模型信息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|iotId|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«deviceModel对象»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|deviceModel对象|deviceModel对象|
|&emsp;&emsp;absorbStatus|扫吸状态|string||
|&emsp;&emsp;aromatherapyStatus|香薰状态，-1代表没有设备，0代表关闭，1-3代表档位|string||
|&emsp;&emsp;cleanArea||string||
|&emsp;&emsp;cleanCount||string||
|&emsp;&emsp;cleanTime||string||
|&emsp;&emsp;cleanWaterBoxStatus|清水箱余水量，取值百分比（%）|string||
|&emsp;&emsp;completeCleanCount||string||
|&emsp;&emsp;deviceProject||string||
|&emsp;&emsp;deviceRosVersion||string||
|&emsp;&emsp;dirtyWaterBoxStatus|污水箱状态。0：未满，1：满|string||
|&emsp;&emsp;dragStatus|拖头状态（600使用）|string||
|&emsp;&emsp;dustBoxStatus||string||
|&emsp;&emsp;ecuHardVersion||string||
|&emsp;&emsp;ecuSoftVersion||string||
|&emsp;&emsp;electric||string||
|&emsp;&emsp;geoLocation||string||
|&emsp;&emsp;geoName||string||
|&emsp;&emsp;hasBaseStation|是否有基站。 0：没有，1：有|string||
|&emsp;&emsp;hasFaceRecognition|是否有人脸识别。 0：没有，1：有|string||
|&emsp;&emsp;hasKnob|是否有旋钮|string||
|&emsp;&emsp;hasVoice|是否有语音交互。 0：没有，1：有|string||
|&emsp;&emsp;iotId||string||
|&emsp;&emsp;isUrgencyStop||string||
|&emsp;&emsp;lockStatus||string||
|&emsp;&emsp;mapName||string||
|&emsp;&emsp;nebulaVersion||string||
|&emsp;&emsp;padVersion||string||
|&emsp;&emsp;pushStatus|尘推状态|string||
|&emsp;&emsp;rainSnowMode||boolean||
|&emsp;&emsp;sweepStatus|湿拖状态|string||
|&emsp;&emsp;waterBoxStatus|是否装载水箱（600使用）|string||
|&emsp;&emsp;workStatusCode||string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"absorbStatus": "",
		"aromatherapyStatus": "",
		"cleanArea": "",
		"cleanCount": "",
		"cleanTime": "",
		"cleanWaterBoxStatus": "",
		"completeCleanCount": "",
		"deviceProject": "",
		"deviceRosVersion": "",
		"dirtyWaterBoxStatus": "",
		"dragStatus": "",
		"dustBoxStatus": "",
		"ecuHardVersion": "",
		"ecuSoftVersion": "",
		"electric": "",
		"geoLocation": "",
		"geoName": "",
		"hasBaseStation": "",
		"hasFaceRecognition": "",
		"hasKnob": "",
		"hasVoice": "",
		"iotId": "",
		"isUrgencyStop": "",
		"lockStatus": "",
		"mapName": "",
		"nebulaVersion": "",
		"padVersion": "",
		"pushStatus": "",
		"rainSnowMode": true,
		"sweepStatus": "",
		"waterBoxStatus": "",
		"workStatusCode": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 更新设备的经纬度和地理名称


**接口地址**:`/nebula/device/module/update-geo`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>更新设备的经纬度和地理名称</p>



**请求示例**:


```javascript
{
  "deviceName": "",
  "geoName": "",
  "latitude": 0,
  "longitude": 0
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceGeoDto|deviceGeoDto|body|true|DeviceGeoDto|DeviceGeoDto|
|&emsp;&emsp;deviceName|设备名称||false|string||
|&emsp;&emsp;geoName|代理位置名称||false|string||
|&emsp;&emsp;latitude|纬度||false|number(double)||
|&emsp;&emsp;longitude|经度||false|number(double)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 设备模块版本


## 新建


**接口地址**:`/nebula/device/module/version/create`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>新建版本对应关系</p>



**请求示例**:


```javascript
{
  "appVersion": "",
  "deviceRosVersion": "",
  "ecuHardVersion": "",
  "ecuSoftVersion": "",
  "id": "",
  "nebulaVersion": "",
  "padVersion": "",
  "sprint": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|DeviceModuleVersionDto|DeviceModuleVersionDto|
|&emsp;&emsp;appVersion|app版本||false|string||
|&emsp;&emsp;deviceRosVersion|core版本||false|string||
|&emsp;&emsp;ecuHardVersion|ecu硬件版本||false|string||
|&emsp;&emsp;ecuSoftVersion|ecu软件版本||false|string||
|&emsp;&emsp;id|设备主键||false|string||
|&emsp;&emsp;nebulaVersion|nebula版本||false|string||
|&emsp;&emsp;padVersion|pad版本||false|string||
|&emsp;&emsp;sprint|冲刺周期||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除


**接口地址**:`/nebula/device/module/version/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 列表


**接口地址**:`/nebula/device/module/version/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|sprint|冲刺周期|query|false|string||
|version|版本号|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«DeviceModuleVersionVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«DeviceModuleVersionVo»|IPage«DeviceModuleVersionVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|DeviceModuleVersionVo|
|&emsp;&emsp;&emsp;&emsp;appVersion|app版本|string||
|&emsp;&emsp;&emsp;&emsp;deviceRosVersion|core版本|string||
|&emsp;&emsp;&emsp;&emsp;ecuHardVersion|ecu硬件版本|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftVersion|ecu软件版本|string||
|&emsp;&emsp;&emsp;&emsp;id|设备主键|string||
|&emsp;&emsp;&emsp;&emsp;nebulaVersion|nebula版本|string||
|&emsp;&emsp;&emsp;&emsp;padVersion|pad版本|string||
|&emsp;&emsp;&emsp;&emsp;sprint|冲刺周期|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"appVersion": "",
				"deviceRosVersion": "",
				"ecuHardVersion": "",
				"ecuSoftVersion": "",
				"id": "",
				"nebulaVersion": "",
				"padVersion": "",
				"sprint": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 查询


**接口地址**:`/nebula/device/module/version/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«DeviceModuleVersionVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|DeviceModuleVersionVo|DeviceModuleVersionVo|
|&emsp;&emsp;appVersion|app版本|string||
|&emsp;&emsp;deviceRosVersion|core版本|string||
|&emsp;&emsp;ecuHardVersion|ecu硬件版本|string||
|&emsp;&emsp;ecuSoftVersion|ecu软件版本|string||
|&emsp;&emsp;id|设备主键|string||
|&emsp;&emsp;nebulaVersion|nebula版本|string||
|&emsp;&emsp;padVersion|pad版本|string||
|&emsp;&emsp;sprint|冲刺周期|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"appVersion": "",
		"deviceRosVersion": "",
		"ecuHardVersion": "",
		"ecuSoftVersion": "",
		"id": "",
		"nebulaVersion": "",
		"padVersion": "",
		"sprint": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 更新


**接口地址**:`/nebula/device/module/version/update`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>更新</p>



**请求示例**:


```javascript
{
  "appVersion": "",
  "deviceRosVersion": "",
  "ecuHardVersion": "",
  "ecuSoftVersion": "",
  "id": "",
  "nebulaVersion": "",
  "padVersion": "",
  "sprint": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|DeviceModuleVersionDto|DeviceModuleVersionDto|
|&emsp;&emsp;appVersion|app版本||false|string||
|&emsp;&emsp;deviceRosVersion|core版本||false|string||
|&emsp;&emsp;ecuHardVersion|ecu硬件版本||false|string||
|&emsp;&emsp;ecuSoftVersion|ecu软件版本||false|string||
|&emsp;&emsp;id|设备主键||false|string||
|&emsp;&emsp;nebulaVersion|nebula版本||false|string||
|&emsp;&emsp;padVersion|pad版本||false|string||
|&emsp;&emsp;sprint|冲刺周期||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 设备管理


## 增加物联网卡号


**接口地址**:`/nebula/device/addIotNum`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>增加物联网卡号</p>



**请求示例**:


```javascript
{
  "deviceName": "",
  "iotNum": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|DeviceIotNumDto|DeviceIotNumDto|
|&emsp;&emsp;deviceName|设备编号||false|string||
|&emsp;&emsp;iotNum|卡号||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 唤醒


**接口地址**:`/nebula/device/awaken`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>唤醒</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 校验设备关联项目


**接口地址**:`/nebula/device/check-relation-project`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>校验设备关联项目</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|companyName|公司名称|query|true|string||
|depName|项目名称|query|true|string||
|deviceName|设备编号|query|true|string||
|companyId|公司id|query|false|string||
|depId|项目id|query|false|string||
|nickName|设备名称|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 校验用户是否有权限修改地理位置


**接口地址**:`/nebula/device/check/updateGeoName`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>校验用户是否有权限修改地理位置</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 校验设备是否能进入远程建图


**接口地址**:`/nebula/device/checkBuildMap`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>校验设备是否能进入远程建图</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 校验机器人是否绑定项目


**接口地址**:`/nebula/device/checkDeviceRelationProject`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>校验机器人是否绑定项目</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 默认设备备注


**接口地址**:`/nebula/device/defult-nickname`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>默认设备备注</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名|query|true|string||
|projectId|项目id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除物联网卡号


**接口地址**:`/nebula/device/delIotNum`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除物联网卡号</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除闸机


**接口地址**:`/nebula/device/deleteDeviceMapGate`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除闸机</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|id|闸机id|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 删除地图


**接口地址**:`/nebula/device/deleteMap`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>删除地图</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|mapId|地图id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 模糊查询部门名称


**接口地址**:`/nebula/device/depart-name/map`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>模糊查询部门名称</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名|query|true|string||
|departName|部门名称|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«List»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|array||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": [],
	"success": true,
	"timestamp": 0
}
```


## 获取设备详情


**接口地址**:`/nebula/device/device-detail`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备详情</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据设备ids返回设备信息


**接口地址**:`/nebula/device/devices/iotids`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>根据设备ids返回设备信息</p>



**请求示例**:


```javascript
{
  "iotIds": []
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|PositionMapDto|PositionMapDto|
|&emsp;&emsp;iotIds|||false|array|string|


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 机器人远程诊断


**接口地址**:`/nebula/device/diagnose`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>机器人远程诊断</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 设置禁区或者虚拟墙


**接口地址**:`/nebula/device/editMap`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>设置禁区或者虚拟墙</p>



**请求示例**:


```javascript
{
  "deviceName": "",
  "feasibleZone": [],
  "id": "",
  "params": [],
  "url": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|DeviceEditMapDto|DeviceEditMapDto|
|&emsp;&emsp;deviceName|设备编号||false|string||
|&emsp;&emsp;feasibleZone|橡皮擦||false|array|object|
|&emsp;&emsp;id|地图id||false|string||
|&emsp;&emsp;params|禁区和虚拟墙||false|array|object|
|&emsp;&emsp;url|||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 使能电机


**接口地址**:`/nebula/device/enableMotor`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>使能电机</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|operationLogId|操作记录id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 结束远程建图


**接口地址**:`/nebula/device/endBuildMap`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>结束远程建图</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|isSave|是否保存|query|true|boolean||
|mapName|地图名称|query|false|string||
|newMap|是否是新地图|query|false|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 导出设备列表


**接口地址**:`/nebula/device/export`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>导出设备列表</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|deviceName|query|false|string||
|deviceStatus|deviceStatus|query|false|string||
|nickName|nickName|query|false|string||
|projectName|projectName|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


暂无


**响应示例**:
```javascript

```


## 获取建图端


**接口地址**:`/nebula/device/getBuildMapType`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取建图端</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据用户权限获取账号下设备


**接口地址**:`/nebula/device/getDeviceByUser`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据用户权限获取账号下设备</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据用户权限获取账号下设备的core版本


**接口地址**:`/nebula/device/getDeviceCoreByUser`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据用户权限获取账号下设备的core版本</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取物联网卡号


**接口地址**:`/nebula/device/getIotNumAndSecret`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取物联网卡号</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 在线状态历史数据处理


**接口地址**:`/nebula/device/history-log-handle`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取项目下的所有设备</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 设备信息


**接口地址**:`/nebula/device/info`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>设备信息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«DeviceInfoVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|DeviceInfoVo|DeviceInfoVo|
|&emsp;&emsp;absorbStatus|扫吸状态|string||
|&emsp;&emsp;cleanWaterBoxStatus|清水箱余水量，取值百分比（%）|string||
|&emsp;&emsp;deviceName|设备编号|string||
|&emsp;&emsp;deviceStatus|设备状态|string||
|&emsp;&emsp;dirtyWaterBoxStatus|污水箱状态。0：未满，1：满|string||
|&emsp;&emsp;electric|电池电量|string||
|&emsp;&emsp;id||string||
|&emsp;&emsp;nickname|名称|string||
|&emsp;&emsp;projectName|项目名称|string||
|&emsp;&emsp;pushStatus|尘推状态|string||
|&emsp;&emsp;sweepStatus|湿拖状态|string||
|&emsp;&emsp;workStatus|工作状态|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"absorbStatus": "",
		"cleanWaterBoxStatus": "",
		"deviceName": "",
		"deviceStatus": "",
		"dirtyWaterBoxStatus": "",
		"electric": "",
		"id": "",
		"nickname": "",
		"projectName": "",
		"pushStatus": "",
		"sweepStatus": "",
		"workStatus": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 设备是否在线


**接口地址**:`/nebula/device/is-online`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>设备是否在线</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 分页列表查询


**接口地址**:`/nebula/device/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|deviceName|query|false|string||
|deviceStatus|deviceStatus|query|false|string||
|iotNum|iotNum|query|false|string||
|nickName|nickName|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|projectName|projectName|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«DeviceVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«DeviceVo»|IPage«DeviceVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|DeviceVo|
|&emsp;&emsp;&emsp;&emsp;depId||string||
|&emsp;&emsp;&emsp;&emsp;deviceName||string||
|&emsp;&emsp;&emsp;&emsp;deviceProject||string||
|&emsp;&emsp;&emsp;&emsp;deviceRosVersion||string||
|&emsp;&emsp;&emsp;&emsp;deviceStatus||string||
|&emsp;&emsp;&emsp;&emsp;electric||string||
|&emsp;&emsp;&emsp;&emsp;geoName||string||
|&emsp;&emsp;&emsp;&emsp;gmtOnline||string||
|&emsp;&emsp;&emsp;&emsp;icon||string||
|&emsp;&emsp;&emsp;&emsp;iotId||string||
|&emsp;&emsp;&emsp;&emsp;iotNum||string||
|&emsp;&emsp;&emsp;&emsp;logFileUpdateTime||string||
|&emsp;&emsp;&emsp;&emsp;nickname||string||
|&emsp;&emsp;&emsp;&emsp;productName||string||
|&emsp;&emsp;&emsp;&emsp;rainSnowMode||boolean||
|&emsp;&emsp;&emsp;&emsp;workStatusCode||string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"depId": "",
				"deviceName": "",
				"deviceProject": "",
				"deviceRosVersion": "",
				"deviceStatus": "",
				"electric": "",
				"geoName": "",
				"gmtOnline": "",
				"icon": "",
				"iotId": "",
				"iotNum": "",
				"logFileUpdateTime": "",
				"nickname": "",
				"productName": "",
				"rainSnowMode": true,
				"workStatusCode": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 控制机器人手动推行


**接口地址**:`/nebula/device/manualPush`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>控制机器人手动推行</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|params|是否推行0：代表不可推行，2：代表可推行|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 修改地图名称


**接口地址**:`/nebula/device/modifyMapName`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>修改地图名称</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|mapId|地图id|query|true|string||
|mapName|地图名称|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 运营专员扫码设备换绑


**接口地址**:`/nebula/device/ouser/device-binding`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>运营专员扫码设备换绑</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 暂停清洁任务


**接口地址**:`/nebula/device/pauseTask`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>暂停清洁任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|operationLogId|操作记录id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 执行任务


**接口地址**:`/nebula/device/performTask`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>执行任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|executeNum|执行次数|query|true|integer(int32)||
|id|任务id|query|true|integer(int64)||
|operationLogId|操作日志id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取所有设备经纬度


**接口地址**:`/nebula/device/position/map`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取所有设备经纬度</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 模糊查询项目名称


**接口地址**:`/nebula/device/project/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>模糊查询项目名称</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|projectName|projectName|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取项目下的所有设备


**接口地址**:`/nebula/device/projectDevice`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取项目下的所有设备</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|project|项目Id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据设备关键字模糊查询相关的设备名称或设备别名的信息


**接口地址**:`/nebula/device/query/keyword`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|keyword|keyword|query|true|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«DeviceVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«DeviceVo»|IPage«DeviceVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|DeviceVo|
|&emsp;&emsp;&emsp;&emsp;depId||string||
|&emsp;&emsp;&emsp;&emsp;deviceName||string||
|&emsp;&emsp;&emsp;&emsp;deviceProject||string||
|&emsp;&emsp;&emsp;&emsp;deviceRosVersion||string||
|&emsp;&emsp;&emsp;&emsp;deviceStatus||string||
|&emsp;&emsp;&emsp;&emsp;electric||string||
|&emsp;&emsp;&emsp;&emsp;geoName||string||
|&emsp;&emsp;&emsp;&emsp;gmtOnline||string||
|&emsp;&emsp;&emsp;&emsp;icon||string||
|&emsp;&emsp;&emsp;&emsp;iotId||string||
|&emsp;&emsp;&emsp;&emsp;iotNum||string||
|&emsp;&emsp;&emsp;&emsp;logFileUpdateTime||string||
|&emsp;&emsp;&emsp;&emsp;nickname||string||
|&emsp;&emsp;&emsp;&emsp;productName||string||
|&emsp;&emsp;&emsp;&emsp;rainSnowMode||boolean||
|&emsp;&emsp;&emsp;&emsp;workStatusCode||string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"depId": "",
				"deviceName": "",
				"deviceProject": "",
				"deviceRosVersion": "",
				"deviceStatus": "",
				"electric": "",
				"geoName": "",
				"gmtOnline": "",
				"icon": "",
				"iotId": "",
				"iotNum": "",
				"logFileUpdateTime": "",
				"nickname": "",
				"productName": "",
				"rainSnowMode": true,
				"workStatusCode": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 根据产品获取设备


**接口地址**:`/nebula/device/queueByProduct`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据产品获取设备</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|firmwareId|升级包id|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|productKey|产品id|query|true|string||
|version|版本|query|true|string||
|versionType|版本类型|query|true|string||
|core|core版本号|query|false|string||
|deviceNames|已选设备|query|false|string||
|ecu|ecu版本号|query|false|string||
|nebula|nebula版本号|query|false|string||
|nickname|设备名|query|false|string||
|pad|core版本号|query|false|string||
|projectId|项目id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«DeviceEditionPageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«DeviceEditionPageVo»|IPage«DeviceEditionPageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|DeviceEditionPageVo|
|&emsp;&emsp;&emsp;&emsp;deviceName|设备编号|string||
|&emsp;&emsp;&emsp;&emsp;deviceRosVersion|设备core版本|string||
|&emsp;&emsp;&emsp;&emsp;ecuSoftVersion|设备ecu版本|string||
|&emsp;&emsp;&emsp;&emsp;geoName|位置|string||
|&emsp;&emsp;&emsp;&emsp;iotId|设备主键|string||
|&emsp;&emsp;&emsp;&emsp;isCheck|是否可以勾选|boolean||
|&emsp;&emsp;&emsp;&emsp;nebulaVersion|设备nebula版本|string||
|&emsp;&emsp;&emsp;&emsp;nickname|设备名|string||
|&emsp;&emsp;&emsp;&emsp;padVersion|设备pad版本|string||
|&emsp;&emsp;&emsp;&emsp;projectName|项目|string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"deviceName": "",
				"deviceRosVersion": "",
				"ecuSoftVersion": "",
				"geoName": "",
				"iotId": "",
				"isCheck": true,
				"nebulaVersion": "",
				"nickname": "",
				"padVersion": "",
				"projectName": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 获取设备软件版本


**接口地址**:`/nebula/device/queueDeviceVersions`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取设备软件版本</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«DeviceVersionsVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|DeviceVersionsVo|DeviceVersionsVo|
|&emsp;&emsp;coreVersionList|core版本|array|string|
|&emsp;&emsp;ecuVersionList|ecu版本|array|string|
|&emsp;&emsp;nebulaVersionList|nebula版本|array|string|
|&emsp;&emsp;padVersionList|pad版本|array|string|
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"coreVersionList": [],
		"ecuVersionList": [],
		"nebulaVersionList": [],
		"padVersionList": []
	},
	"success": true,
	"timestamp": 0
}
```


## 获取机器人实时位姿


**接口地址**:`/nebula/device/real/pose`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取机器人实时位姿</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 设备重启


**接口地址**:`/nebula/device/reboot`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>设备重启</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|operationLogId|操作记录id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 设备关联项目


**接口地址**:`/nebula/device/relation-project`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>设备关联项目</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|iotId|设备id|query|true|||
|iotIds|iotIds|query|true|string||
|depId|部门id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 重定位


**接口地址**:`/nebula/device/relocation`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>重定位</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|operationLogId|操作记录id|query|true|string||
|x|x|query|true|number(double)||
|y|y|query|true|number(double)||
|yaw|yaw|query|true|number(double)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 恢复清洁任务


**接口地址**:`/nebula/device/resumeTask`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>恢复清洁任务</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|operationLogId|操作记录id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 返回基站


**接口地址**:`/nebula/device/returnStation`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>返回基站</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|operationLogId|操作记录id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 更新设备绑定的项目、公司名称


**接口地址**:`/nebula/device/save-project`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>更新设备绑定的项目、公司名称</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|departId|部门Id 只在部门换绑时传入|query|false|string||
|departName|部门名称 只在新建部门时传入|query|false|string||
|projectName|公司名称 更新公司名称时传入|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 开启关闭雨雪天模式


**接口地址**:`/nebula/device/setRainSnow`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>开启关闭雨雪天模式</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|rainSnowStatus|状态,true开启，false关闭|query|true|boolean||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 整机关机


**接口地址**:`/nebula/device/shutdown`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>整机关机</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名称|query|true|string||
|operationLogId|操作记录id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 开始远程建图


**接口地址**:`/nebula/device/startBuildMap`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>开始远程建图</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|type|类型|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 同步设备


**接口地址**:`/nebula/device/sync`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>同步设备</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 修改设备备注


**接口地址**:`/nebula/device/update-nickname`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>修改设备备注</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备名|query|true|string||
|nickName|备注|query|true|string||
|iotId|设备id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 修改闸机信息


**接口地址**:`/nebula/device/updateDeviceMapGate`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>修改闸机信息</p>



**请求示例**:


```javascript
{
  "deviceName": "",
  "endX": 0,
  "endY": 0,
  "factoryId": "",
  "id": 0,
  "leftGateId": "",
  "omapId": "",
  "rightGateId": "",
  "startX": 0,
  "startY": 0
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|DeviceMapGateUpdateDto|DeviceMapGateUpdateDto|
|&emsp;&emsp;deviceName|设备编号||false|string||
|&emsp;&emsp;endX|闸机线终点的X坐标||false|integer(int32)||
|&emsp;&emsp;endY|闸机线终点的Y坐标||false|integer(int32)||
|&emsp;&emsp;factoryId|闸机厂商ID||false|string||
|&emsp;&emsp;id|闸机内部编号||false|integer(int32)||
|&emsp;&emsp;leftGateId|左闸机点对应的uuid||false|string||
|&emsp;&emsp;omapId|||false|string||
|&emsp;&emsp;rightGateId|右闸机点对应的uuid||false|string||
|&emsp;&emsp;startX|闸机线起点的X坐标||false|integer(int32)||
|&emsp;&emsp;startY|闸机线起点的Y坐标||false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 修改物联网卡号


**接口地址**:`/nebula/device/updateIotNum`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>修改物联网卡号</p>



**请求示例**:


```javascript
{
  "deviceName": "",
  "iotNum": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|DeviceIotNumDto|DeviceIotNumDto|
|&emsp;&emsp;deviceName|设备编号||false|string||
|&emsp;&emsp;iotNum|卡号||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 设备绑定


## 设备绑定


**接口地址**:`/nebula/device/binding`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:


**请求示例**:


```javascript
{
  "companyId": "",
  "companyName": "",
  "deviceName": "",
  "nickName": "",
  "projectId": "",
  "projectName": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceBindDto|deviceBindDto|body|true|DeviceBindDto|DeviceBindDto|
|&emsp;&emsp;companyId|公司Id||false|string||
|&emsp;&emsp;companyName|公司名称||false|string||
|&emsp;&emsp;deviceName|设备名称||false|string||
|&emsp;&emsp;nickName|设备别名||false|string||
|&emsp;&emsp;projectId|项目ID||false|string||
|&emsp;&emsp;projectName|项目名称||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«BindingResponseVo»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|BindingResponseVo|BindingResponseVo|
|&emsp;&emsp;code|状态码|integer(int32)||
|&emsp;&emsp;companyInput|公司输入框能否输入 ture -> 可以输入|boolean||
|&emsp;&emsp;deviceProjectAndCompanyVo|设备信息|DeviceProjectAndCompanyVo|DeviceProjectAndCompanyVo|
|&emsp;&emsp;&emsp;&emsp;companyId|设备所在公司Id|string||
|&emsp;&emsp;&emsp;&emsp;companyName|设备所在公司|string||
|&emsp;&emsp;&emsp;&emsp;deviceName|设备名|string||
|&emsp;&emsp;&emsp;&emsp;iotId|设备Id|string||
|&emsp;&emsp;&emsp;&emsp;nickName|设备别名|string||
|&emsp;&emsp;&emsp;&emsp;projectId|设备所在项目Id|string||
|&emsp;&emsp;&emsp;&emsp;projectName|设备所在项目|string||
|&emsp;&emsp;isInterrupt|流程是否中断 ture -> 中断|boolean||
|&emsp;&emsp;message|提示信息|string||
|&emsp;&emsp;projectInput|项目输入框能否输入 ture -> 可以输入|boolean||
|&emsp;&emsp;useDeviceInfo|是否使用设备的公司和项目 true -> 使用|boolean||
|&emsp;&emsp;useUserInfo|是否使用用户的信息 true -> 使用|boolean||
|&emsp;&emsp;userProjectAndCompanyVo|用户信息|UserProjectAndCompanyVo|UserProjectAndCompanyVo|
|&emsp;&emsp;&emsp;&emsp;companyId|用户所在公司Id|string||
|&emsp;&emsp;&emsp;&emsp;companyName|用户所在公司|string||
|&emsp;&emsp;&emsp;&emsp;projectId|用户所在项目Id|string||
|&emsp;&emsp;&emsp;&emsp;projectName|用户所在项目|string||
|&emsp;&emsp;&emsp;&emsp;username|用户名|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"code": 0,
		"companyInput": true,
		"deviceProjectAndCompanyVo": {
			"companyId": "",
			"companyName": "",
			"deviceName": "",
			"iotId": "",
			"nickName": "",
			"projectId": "",
			"projectName": ""
		},
		"isInterrupt": true,
		"message": "",
		"projectInput": true,
		"useDeviceInfo": true,
		"useUserInfo": true,
		"userProjectAndCompanyVo": {
			"companyId": "",
			"companyName": "",
			"projectId": "",
			"projectName": "",
			"username": ""
		}
	},
	"success": true,
	"timestamp": 0
}
```


## 扫码绑定检查


**接口地址**:`/nebula/device/binding/check`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|deviceName|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«BindingResponseVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|BindingResponseVo|BindingResponseVo|
|&emsp;&emsp;code|状态码|integer(int32)||
|&emsp;&emsp;companyInput|公司输入框能否输入 ture -> 可以输入|boolean||
|&emsp;&emsp;deviceProjectAndCompanyVo|设备信息|DeviceProjectAndCompanyVo|DeviceProjectAndCompanyVo|
|&emsp;&emsp;&emsp;&emsp;companyId|设备所在公司Id|string||
|&emsp;&emsp;&emsp;&emsp;companyName|设备所在公司|string||
|&emsp;&emsp;&emsp;&emsp;deviceName|设备名|string||
|&emsp;&emsp;&emsp;&emsp;iotId|设备Id|string||
|&emsp;&emsp;&emsp;&emsp;nickName|设备别名|string||
|&emsp;&emsp;&emsp;&emsp;projectId|设备所在项目Id|string||
|&emsp;&emsp;&emsp;&emsp;projectName|设备所在项目|string||
|&emsp;&emsp;isInterrupt|流程是否中断 ture -> 中断|boolean||
|&emsp;&emsp;message|提示信息|string||
|&emsp;&emsp;projectInput|项目输入框能否输入 ture -> 可以输入|boolean||
|&emsp;&emsp;useDeviceInfo|是否使用设备的公司和项目 true -> 使用|boolean||
|&emsp;&emsp;useUserInfo|是否使用用户的信息 true -> 使用|boolean||
|&emsp;&emsp;userProjectAndCompanyVo|用户信息|UserProjectAndCompanyVo|UserProjectAndCompanyVo|
|&emsp;&emsp;&emsp;&emsp;companyId|用户所在公司Id|string||
|&emsp;&emsp;&emsp;&emsp;companyName|用户所在公司|string||
|&emsp;&emsp;&emsp;&emsp;projectId|用户所在项目Id|string||
|&emsp;&emsp;&emsp;&emsp;projectName|用户所在项目|string||
|&emsp;&emsp;&emsp;&emsp;username|用户名|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"code": 0,
		"companyInput": true,
		"deviceProjectAndCompanyVo": {
			"companyId": "",
			"companyName": "",
			"deviceName": "",
			"iotId": "",
			"nickName": "",
			"projectId": "",
			"projectName": ""
		},
		"isInterrupt": true,
		"message": "",
		"projectInput": true,
		"useDeviceInfo": true,
		"useUserInfo": true,
		"userProjectAndCompanyVo": {
			"companyId": "",
			"companyName": "",
			"projectId": "",
			"projectName": "",
			"username": ""
		}
	},
	"success": true,
	"timestamp": 0
}
```


# 部门权限表


## 部门权限表-添加


**接口地址**:`/nebula/sys/sysDepartPermission/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>部门权限表-添加</p>



**请求示例**:


```javascript
{
  "dataRuleIds": "",
  "departId": "",
  "id": "",
  "permissionId": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysDepartPermission|部门权限表|body|true|sys_depart_permission对象|sys_depart_permission对象|
|&emsp;&emsp;dataRuleIds|数据规则id||false|string||
|&emsp;&emsp;departId|部门id||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;permissionId|权限id||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门权限表-通过id删除


**接口地址**:`/nebula/sys/sysDepartPermission/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>部门权限表-通过id删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门权限表-批量删除


**接口地址**:`/nebula/sys/sysDepartPermission/deleteBatch`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>部门权限表-批量删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|ids|ids|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门权限表-编辑


**接口地址**:`/nebula/sys/sysDepartPermission/edit`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>部门权限表-编辑</p>



**请求示例**:


```javascript
{
  "dataRuleIds": "",
  "departId": "",
  "id": "",
  "permissionId": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysDepartPermission|部门权限表|body|true|sys_depart_permission对象|sys_depart_permission对象|
|&emsp;&emsp;dataRuleIds|数据规则id||false|string||
|&emsp;&emsp;departId|部门id||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;permissionId|权限id||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门权限表-编辑


**接口地址**:`/nebula/sys/sysDepartPermission/edit`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>部门权限表-编辑</p>



**请求示例**:


```javascript
{
  "dataRuleIds": "",
  "departId": "",
  "id": "",
  "permissionId": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysDepartPermission|部门权限表|body|true|sys_depart_permission对象|sys_depart_permission对象|
|&emsp;&emsp;dataRuleIds|数据规则id||false|string||
|&emsp;&emsp;departId|部门id||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;permissionId|权限id||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门权限表-分页列表查询


**接口地址**:`/nebula/sys/sysDepartPermission/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>部门权限表-分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dataRuleIds|数据规则id|query|false|string||
|departId|部门id|query|false|string||
|id|id|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|permissionId|权限id|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门权限表-通过id查询


**接口地址**:`/nebula/sys/sysDepartPermission/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>部门权限表-通过id查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 部门管理


## 微信用户新增公司


**接口地址**:`/nebula/sys/sysDepart/create-company`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>微信用户新增公司</p>



**请求示例**:


```javascript
{
  "departName": "",
  "departOrder": 0,
  "orgCategory": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|addCompanyDto|addCompanyDto|body|true|AddCompanyDto|AddCompanyDto|
|&emsp;&emsp;departName|公司名称||true|string||
|&emsp;&emsp;departOrder|排序||true|integer(int32)||
|&emsp;&emsp;orgCategory|机构类别，默认传1||true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 微信用户新增项目


**接口地址**:`/nebula/sys/sysDepart/create-project`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>微信用户新增项目</p>



**请求示例**:


```javascript
{
  "departName": "",
  "departOrder": 0,
  "orgCategory": "",
  "parentId": "",
  "tag": 0
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|addProjectDto|addProjectDto|body|true|AddProjectDto|AddProjectDto|
|&emsp;&emsp;departName|项目名称||true|string||
|&emsp;&emsp;departOrder|排序||true|integer(int32)||
|&emsp;&emsp;orgCategory|机构类别，默认传2||true|string||
|&emsp;&emsp;parentId|公司id||true|string||
|&emsp;&emsp;tag|||false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据公司名查询项目


**接口地址**:`/nebula/sys/sysDepart/queryByCompany`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据公司名查询项目</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|companyId|公司id|query|true|string||
|companyName|公司名称|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 查询用户所属公司


**接口地址**:`/nebula/sys/sysDepart/queryDepartList`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询用户所属公司</p>



**请求参数**:


暂无


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 小程序数据总览公司 项目 设备查询 


**接口地址**:`/nebula/sys/sysDepart/queryMap`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>小程序数据总览公司 项目 设备查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|请求id|query|false|string||
|type|类型 空 company project|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 模糊查询项目名称


**接口地址**:`/nebula/sys/sysDepart/queryProject/keyword`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|keyword|keyword|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据公司查询下面的项目


**接口地址**:`/nebula/sys/sysDepart/queryProjectsByCompany`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据公司查询下面的项目</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|companyId|公司id|query|true|string||
|projectName|项目名|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 部门角色


## 部门角色-添加


**接口地址**:`/nebula/sys/sysDepartRole/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>部门角色-添加</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "departId": "",
  "description": "",
  "id": "",
  "roleCode": "",
  "roleName": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysDepartRole|部门角色|body|true|sys_depart_role对象|sys_depart_role对象|
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;departId|部门id||false|string||
|&emsp;&emsp;description|描述||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;roleCode|部门角色编码||false|string||
|&emsp;&emsp;roleName|部门角色名称||false|string||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门角色-通过id删除


**接口地址**:`/nebula/sys/sysDepartRole/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>部门角色-通过id删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门角色-批量删除


**接口地址**:`/nebula/sys/sysDepartRole/deleteBatch`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>部门角色-批量删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|ids|ids|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门角色-编辑


**接口地址**:`/nebula/sys/sysDepartRole/edit`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>部门角色-编辑</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "departId": "",
  "description": "",
  "id": "",
  "roleCode": "",
  "roleName": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysDepartRole|部门角色|body|true|sys_depart_role对象|sys_depart_role对象|
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;departId|部门id||false|string||
|&emsp;&emsp;description|描述||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;roleCode|部门角色编码||false|string||
|&emsp;&emsp;roleName|部门角色名称||false|string||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门角色-编辑


**接口地址**:`/nebula/sys/sysDepartRole/edit`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>部门角色-编辑</p>



**请求示例**:


```javascript
{
  "createBy": "",
  "createTime": "",
  "departId": "",
  "description": "",
  "id": "",
  "roleCode": "",
  "roleName": "",
  "updateBy": "",
  "updateTime": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|sysDepartRole|部门角色|body|true|sys_depart_role对象|sys_depart_role对象|
|&emsp;&emsp;createBy|创建人||false|string||
|&emsp;&emsp;createTime|创建时间||false|string(date-time)||
|&emsp;&emsp;departId|部门id||false|string||
|&emsp;&emsp;description|描述||false|string||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;roleCode|部门角色编码||false|string||
|&emsp;&emsp;roleName|部门角色名称||false|string||
|&emsp;&emsp;updateBy|更新人||false|string||
|&emsp;&emsp;updateTime|更新时间||false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门角色-分页列表查询


**接口地址**:`/nebula/sys/sysDepartRole/list`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>部门角色-分页列表查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|createBy|创建人|query|false|string||
|createTime|创建时间|query|false|string(date-time)||
|departId|部门id|query|false|string||
|deptId|deptId|query|false|string||
|description|描述|query|false|string||
|id|id|query|false|string||
|pageNo|pageNo|query|false|integer(int32)||
|pageSize|pageSize|query|false|integer(int32)||
|roleCode|部门角色编码|query|false|string||
|roleName|部门角色名称|query|false|string||
|updateBy|更新人|query|false|string||
|updateTime|更新时间|query|false|string(date-time)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 部门角色-通过id查询


**接口地址**:`/nebula/sys/sysDepartRole/queryById`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>部门角色-通过id查询</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«object»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 重复校验


## 重复校验接口


**接口地址**:`/nebula/sys/duplicate/check`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dataId|数据ID|query|false|string||
|fieldName|字段名|query|false|string||
|fieldVal|字段值|query|false|string||
|tableName|表名|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«string»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": "",
	"success": true,
	"timestamp": 0
}
```


# 闸机控制


## 新增


**接口地址**:`/nebula/gate/add`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>新增</p>



**请求示例**:


```javascript
{
  "controlType": 0,
  "factoryId": "",
  "factoryName": "",
  "gateName": "",
  "gateUuid1": "",
  "gateUuid2": "",
  "projectId": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|GatePageAddDto|GatePageAddDto|
|&emsp;&emsp;controlType|||false|integer(int32)||
|&emsp;&emsp;factoryId|||false|string||
|&emsp;&emsp;factoryName|||false|string||
|&emsp;&emsp;gateName|||false|string||
|&emsp;&emsp;gateUuid1|||false|string||
|&emsp;&emsp;gateUuid2|||false|string||
|&emsp;&emsp;projectId|||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 关闸机


**接口地址**:`/nebula/gate/closeGate`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>关闸机</p>



**请求示例**:


```javascript
{
  "deviceName": "",
  "factoryId": "",
  "uuid": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|GateDto|GateDto|
|&emsp;&emsp;deviceName|||false|string||
|&emsp;&emsp;factoryId|||false|string||
|&emsp;&emsp;uuid|||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 通过id删除


**接口地址**:`/nebula/gate/delete`


**请求方式**:`DELETE`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>通过id删除</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|204|No Content||
|401|Unauthorized||
|403|Forbidden||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 根据设备编号查询可用的闸机信息


**接口地址**:`/nebula/gate/getGateListByDeviceName`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>根据设备编号查询可用的闸机信息</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 获取地图闸机点位


**接口地址**:`/nebula/gate/getMapGatePoint`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>获取地图闸机点位</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|mapId|设备编号|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 开闸机


**接口地址**:`/nebula/gate/openGate`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>开闸机</p>



**请求示例**:


```javascript
{
  "deviceName": "",
  "factoryId": "",
  "uuid": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|GateDto|GateDto|
|&emsp;&emsp;deviceName|||false|string||
|&emsp;&emsp;factoryId|||false|string||
|&emsp;&emsp;uuid|||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 查询所有


**接口地址**:`/nebula/gate/queryAll`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询所有</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|controlType|控制方式 0云端 1硬件|query|false|integer(int32)||
|factoryId|闸机厂家ID|query|false|string||
|factoryName|闸机厂商名称|query|false|string||
|gateUuid|闸机uuid|query|false|string||
|projectId|所属项目|query|false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 查询闸机开关记录


**接口地址**:`/nebula/gate/queryLog`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>查询闸机开关记录</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|deviceName|设备编号|query|true|string||
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


## 通过id修改


**接口地址**:`/nebula/gate/update`


**请求方式**:`PUT`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>通过id修改</p>



**请求示例**:


```javascript
{
  "controlType": 0,
  "factoryId": "",
  "factoryName": "",
  "gateName": "",
  "gateUuid1": "",
  "gateUuid2": "",
  "id": "",
  "projectId": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|GatePageUpdateDto|GatePageUpdateDto|
|&emsp;&emsp;controlType|||false|integer(int32)||
|&emsp;&emsp;factoryId|||false|string||
|&emsp;&emsp;factoryName|||false|string||
|&emsp;&emsp;gateName|||false|string||
|&emsp;&emsp;gateUuid1|||false|string||
|&emsp;&emsp;gateUuid2|||false|string||
|&emsp;&emsp;id|||false|string||
|&emsp;&emsp;projectId|||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```


# 麦岩认证


## 详情


**接口地址**:`/nebula/certificate/info`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>详情</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|id|id|query|true|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«CertificateInfoVo»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|CertificateInfoVo|CertificateInfoVo|
|&emsp;&emsp;grades|认证分数|integer(int32)||
|&emsp;&emsp;id|id|string||
|&emsp;&emsp;materialUrl|认证过程材料|string||
|&emsp;&emsp;res|认证结果1 CRO，2 CRP， 3 CRE|integer(int32)||
|&emsp;&emsp;time|认证时间|string||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"grades": 0,
		"id": "",
		"materialUrl": "",
		"res": 0,
		"time": ""
	},
	"success": true,
	"timestamp": 0
}
```


## 分页


**接口地址**:`/nebula/certificate/page`


**请求方式**:`GET`


**请求数据类型**:`application/x-www-form-urlencoded`


**响应数据类型**:`*/*`


**接口描述**:<p>分页</p>



**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|pageNo|当前页|query|true|integer(int32)||
|pageSize|每页显示数|query|true|integer(int32)||
|name|公司/项目名称|query|false|string||
|phone|电话|query|false|string||
|status|状态|query|false|integer(int32)||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象«IPage«CertificatePageVo»»|
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|IPage«CertificatePageVo»|IPage«CertificatePageVo»|
|&emsp;&emsp;current||integer(int64)||
|&emsp;&emsp;pages||integer(int64)||
|&emsp;&emsp;records||array|CertificatePageVo|
|&emsp;&emsp;&emsp;&emsp;companyName|公司|string||
|&emsp;&emsp;&emsp;&emsp;createTime|时间|integer||
|&emsp;&emsp;&emsp;&emsp;id|id|string||
|&emsp;&emsp;&emsp;&emsp;phone|电话号码|string||
|&emsp;&emsp;&emsp;&emsp;projectName|项目|string||
|&emsp;&emsp;&emsp;&emsp;status|状态0未回复  1已认证|integer||
|&emsp;&emsp;&emsp;&emsp;userId||string||
|&emsp;&emsp;size||integer(int64)||
|&emsp;&emsp;total||integer(int64)||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {
		"current": 0,
		"pages": 0,
		"records": [
			{
				"companyName": "",
				"createTime": 0,
				"id": "",
				"phone": "",
				"projectName": "",
				"status": 0,
				"userId": ""
			}
		],
		"size": 0,
		"total": 0
	},
	"success": true,
	"timestamp": 0
}
```


## 更新


**接口地址**:`/nebula/certificate/update`


**请求方式**:`POST`


**请求数据类型**:`application/json`


**响应数据类型**:`*/*`


**接口描述**:<p>更新</p>



**请求示例**:


```javascript
{
  "grades": 0,
  "id": "",
  "materialUrl": "",
  "res": 0,
  "time": ""
}
```


**请求参数**:


| 参数名称 | 参数说明 | 请求类型    | 是否必须 | 数据类型 | schema |
| -------- | -------- | ----- | -------- | -------- | ------ |
|dto|dto|body|true|CertificatePageDto|CertificatePageDto|
|&emsp;&emsp;grades|认证分数||false|integer(int32)||
|&emsp;&emsp;id|id||false|string||
|&emsp;&emsp;materialUrl|认证过程材料||false|string||
|&emsp;&emsp;res|认证结果1 CRO，2 CRP， 3 CRE||false|integer(int32)||
|&emsp;&emsp;time|认证时间||false|string||


**响应状态**:


| 状态码 | 说明 | schema |
| -------- | -------- | ----- | 
|200|OK|接口返回对象|
|201|Created||
|401|Unauthorized||
|403|Forbidden||
|404|Not Found||


**响应参数**:


| 参数名称 | 参数说明 | 类型 | schema |
| -------- | -------- | ----- |----- | 
|code|返回代码|integer(int32)|integer(int32)|
|message|返回处理消息|string||
|result|返回数据对象|object||
|success|成功标志|boolean||
|timestamp|时间戳|integer(int64)|integer(int64)|


**响应示例**:
```javascript
{
	"code": 0,
	"message": "",
	"result": {},
	"success": true,
	"timestamp": 0
}
```
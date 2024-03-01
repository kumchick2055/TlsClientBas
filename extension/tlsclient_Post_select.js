var Value = GetInputConstructorValue("Value", loader);
if(Value["original"].length <= 0){
    Invalid(tr("Url") + " " + tr("is empty"));
    return;
};
var UseConstructor = $("#Check2").is(':checked');
var Method = GetInputConstructorValue("Method", loader);
if(Method["original"].length <= 0){
	Invalid(tr("Method") + " " + tr("is empty"));
    return;
};
var ContentType = GetInputConstructorValue("ContentType", loader);
if(ContentType["original"].length <= 0){
	Invalid(tr("ContentType") + " " + tr("is empty"));
    return;
};
var PostDataArray = GetInputConstructorValue("PostDataArray", loader);
var Headers = GetInputConstructorValue("Headers", loader);
var PostDataRaw = GetInputConstructorValue("PostDataRaw", loader);
var ContentTypeRaw = GetInputConstructorValue("ContentTypeRaw", loader);
var Redirect = $("#Check").is(':checked');

try{
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_Post_code").html())({
        Value: Value["updated"],
        ContentType: ContentType["updated"],
        Method: Method["updated"],
        UseConstructor: UseConstructor,
        PostDataRaw: PostDataRaw["updated"].replace(/base64:/g, "_browserautomationstudio_base64:"),
        ContentTypeRaw: ContentTypeRaw["updated"],
        Params: "[" + PostDataArray["updated"] + "]",
        Headers: Headers["updated"],
        Redirect: Redirect
    })
	code = Normalize(code, 0);
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
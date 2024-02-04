var Value = GetInputConstructorValue("Value", loader);
if(Value["original"].length <= 0){
	Invalid(tr("Url") + " " + tr("is empty"));
    return;
};
var Headers = GetInputConstructorValue("Headers", loader);
var Method = GetInputConstructorValue("Method", loader);
if(Method["original"].length <= 0){
	Invalid(tr("Method") + " " + tr("is empty"));
    return;
};

var Redirect = $("#Check").is(':checked');

try {
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_Get_code").html())({
        Value: Value["updated"],
		Headers: Headers["updated"],
		Method: Method["updated"],
		Redirect: Redirect,
    })
	code = Normalize(code, 0);
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){Invalid(e);}
var Value = GetInputConstructorValue("Value", loader);
var Url = GetInputConstructorValue("Url", loader);
if(Url["original"].length <= 0){
	Invalid(tr("URL") + " " + tr("is empty"));
    return;
};
try{
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_RestoreCookies_code").html())({
        Value: Value["updated"],
        Url: Url["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}

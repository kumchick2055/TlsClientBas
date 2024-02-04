var Value = GetInputConstructorValue("Timeout", loader);
if (Value["original"].length == 0) {
	Invalid(tr("Header Value") + " " + tr("is empty"));
    return;
};

try{
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_SetTimeout_code").html())({
        Timeout: Value["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
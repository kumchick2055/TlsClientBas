var Name = GetInputConstructorValue("Name",loader)
if (Name["original"].length == 0) {
	Invalid(tr("Header Name") + " " + tr("is empty"));
    return;
};
var Value = GetInputConstructorValue("Value", loader);
if (Value["original"].length == 0) {
	Invalid(tr("Header Value") + " " + tr("is empty"));
    return;
};

try{
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_SetHeaders_code").html())({
		Name: Name["updated"],
        Value: Value["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
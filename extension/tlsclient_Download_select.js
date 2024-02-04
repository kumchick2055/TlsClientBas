var Value = GetInputConstructorValue("Value", loader);
if(Value["original"].length <= 0){
	Invalid(tr("Url") + " " + tr("is empty"));
	return;
};
var File = GetInputConstructorValue("File", loader);
if(File["original"].length <= 0){
	Invalid(tr("File Path") + " " + tr("is empty"));
	return;
};

try{
	var code = loader.GetAdditionalData() + _.template($("#tlsclient_Download_code").html())({
		Value: Value["updated"],
		File: File["updated"],
	})
	code = Normalize(code, 0)
	BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
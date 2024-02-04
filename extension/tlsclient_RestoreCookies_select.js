var Value = GetInputConstructorValue("Value", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_RestoreCookies_code").html())({
        Value: Value["updated"]
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}

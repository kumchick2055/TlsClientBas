var ProxyText = GetInputConstructorValue("ProxyText", loader);
var ProxyType = GetInputConstructorValue("ProxyType", loader);
if (ProxyType["original"].length == 0) {
	Invalid(tr("Proxy Type") + " " + tr("is empty"));
    return;
};
var ProxyLogin = GetInputConstructorValue("ProxyLogin", loader);
var ProxyPassword = GetInputConstructorValue("ProxyPassword", loader);
try{
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_Proxy_code").html())({
        ProxyText: ProxyText["updated"],
        ProxyType: ProxyType["updated"],
		ProxyLogin: ProxyLogin["updated"],
        ProxyPassword: ProxyPassword["updated"],
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
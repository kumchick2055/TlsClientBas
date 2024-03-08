var fingerprint = $("#fingerprint").val();
var randomTlsExtension = $("#Check").is(':checked');

try{
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_ClientSettings_code").html())({
        "fingerprint": "\"" + fingerprint + "\"",
        "randomTlsExtension": randomTlsExtension
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){log(e);}
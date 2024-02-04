var fingerprint = $("#fingerprint").val();

try{
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_Fingerprint_code").html())({
        "fingerprint": "\"" + fingerprint + "\"",
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){log(e);}
var Save = this.$el.find("#Save").val().toUpperCase();
var Decode = $("#Check").is(':checked');

if(Save.length == 0){
	Invalid(tr("The variable to which the result is saved is missing"));
	return;
};

try{
    var code = loader.GetAdditionalData() + _.template($("#tlsclient_Content_code").html())({
        variable: "VAR_" + Save,
        Decode: Decode
    });
    code = Normalize(code, 0);
    BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
}catch(e){}
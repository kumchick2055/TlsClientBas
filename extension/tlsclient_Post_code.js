_do(function(){
    var cycle_index = _iterator();
    var parsedResponse = {};
    
    _call(function(){
        _on_fail(function(){
            VAR_LAST_ERROR = _result();
            VAR_ERROR_ID = ScriptWorker.GetCurrentAction();
            VAR_WAS_ERROR = false;
            _break(1,true);
        });
        CYCLES.Current().RemoveLabel("function");
        
        native_async("tls_client", "Request", JSON.stringify({
            "redirect": (<%= Redirect %>),
            "isByteResponse": false,
            "headers": (<%= Headers %>),
            "url": (<%= Value %>),
            "method": (<%= Method %>),
            "isConstructor": (<%= UseConstructor %>),
            "payloadRaw": (<%= PostDataRaw %>),
            "payloadConstructor": JSON.stringify(<%= Params %>),
            "contentTypeRaw": (<%= ContentTypeRaw %>),
            "contentTypeConstructor": (<%= ContentType %>)
        }))!

        parsedResponse = JSON.parse(_result());

        if(parsedResponse["status"] < 200){
            fail("[TLS Client] Error: " + parsedResponse["body"]);
        }
    },null)!
    
    _if_else(VAR_WAS_ERROR || parsedResponse["status"] == 0, function(){
        if(cycle_index >= (<%= MaximumFailes %>) ){
            if(!VAR_WAS_ERROR && parsedResponse["status"] == 0){
                fail("[TLS Client] Error: " + parsedResponse["body"])
            }else{
                fail(VAR_LAST_ERROR)
            }
        }
        sleep(1000)!
    }, function(){
        _break("function")
    })!
    
})!
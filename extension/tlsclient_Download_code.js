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
            "redirect": true,
            "isByteResponse": false,
            "headers": "",
            "url": (<%= Value %>),
            "method": "GET",
            "downloadPath": (<%= File %>)
        }))!
        
        parsedResponse = JSON.parse(_result());
        
        if(parsedResponse["status"] == 0){
            fail("[TLS Client] Error: " + parsedResponse["body"])
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
_do(function(){
    var cycle_index = _iterator();
    
    _call(function(){
        _on_fail(function(){
            VAR_LAST_ERROR = _result();
            VAR_ERROR_ID = ScriptWorker.GetCurrentAction();
            VAR_WAS_ERROR = false;
            _break(1,true);
        });
        CYCLES.Current().RemoveLabel("function");
        
        native("tls_client", "SetTimeout", <%= Timeout %>);
    },null)!
    
    _if_else(VAR_WAS_ERROR, function(){
        if(cycle_index >= 10 ){
            if(!VAR_WAS_ERROR){
                fail("[TLS Client] Error: " + "Set Timeout")
            }else{
                fail(VAR_LAST_ERROR)
            }
        }
        sleep(1000)!
    }, function(){
        _break("function")
    })!
    
})!
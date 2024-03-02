requestJson = "false";

if( (<%= Decode %>) ){
    requestJson = "true";
}

response = native("tls_client", "GetResponse", requestJson);

<%= variable %> = response;

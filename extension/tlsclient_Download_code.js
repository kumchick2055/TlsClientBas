native_async("tls_client", "Request", JSON.stringify({
    "redirect": true,
    "isByteResponse": false,
    "headers": "",
    "url": (<%= Value %>),
    "method": "GET",
    "downloadPath": (<%= File %>)
}))!


parsedResponse = JSON.parse(_result());

if(parsedResponse["status"] < 200){
	fail("[TLS Client] Error: " + parsedResponse["body"]);
}
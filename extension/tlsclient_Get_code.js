native_async("tls_client", "Request", JSON.stringify({
    "redirect": (<%= Redirect %>),
    "isByteResponse": false,
    "headers": (<%= Headers %>),
    "url": (<%= Value %>),
    "method": (<%= Method %>)
}))!

parsedResponse = JSON.parse(_result());

if(parsedResponse["status"] < 200){
    fail("[TlsClient] Error: " + parsedResponse["body"]);
}

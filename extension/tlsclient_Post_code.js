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

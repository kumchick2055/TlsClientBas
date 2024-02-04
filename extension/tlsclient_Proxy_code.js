var inputJson = {  
    "proxyText": <%= ProxyText %>, 
    "proxyType": <%= ProxyType %>, 
    "proxyLogin": <%= ProxyLogin %>, 
    "proxyPassword": <%= ProxyPassword %>
};

native("tls_client", "SetProxy", JSON.stringify(inputJson));

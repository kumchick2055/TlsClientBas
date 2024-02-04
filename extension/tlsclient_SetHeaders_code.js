var inputJson = {
    key: <%= Name %>,
    value: <%= Value %>
};

native("tls_client", "SetHeader", JSON.stringify(inputJson));
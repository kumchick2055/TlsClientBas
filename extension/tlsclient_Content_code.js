response = JSON.parse(native("tls_client", "GetResponse", ""));

<%= variable %> = response["body"];

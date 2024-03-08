requestJson = {
    "fingerprint": (<%= fingerprint %>),
    "randomTlsExtension": (<%= randomTlsExtension %>)
};

native("tls_client", "SetFingerprint", JSON.stringify(requestJson));
<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
        id:"Name", 
        description:tr("Header Name"), 
        default_selector: "string", 
        disable_int:true, 
        value_string: "User-Agent", 
        variants: ["User-Agent","Accept-Language"], 
        size: 3, 
        help: {description: tr("Request header name. Header name must contain one or several words separated with dash."), examples:[{code:"User-Agent",description:tr("Set 'User-Agent' header.")},{code:"Accept-Language",description:tr("Set 'Accept-Language' header.")},{code:"Referer",description:tr("Set referer.")}]}
    }) %>
    
	<%= _.template($('#input_constructor').html())({
        id:"Value", 
        description:tr("Header Value"), 
        default_selector: "string", 
        disable_int:true, 
        value_string: "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:44.0) Gecko/20100101 Firefox/44.0", 
        help: {description: tr("Request header value. This could be arbitrary string.")}
    }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Set request header for subsequent requests with http client.</div>
	<div class="tr tooltip-paragraph-fold">You can set any valid header, like 'User-Agent', 'Referer', etc. However here is a list of headers that should not be set with this action: 'Connection', 'Host', 'Content-Type'. 'Content-Type' must be set in 'POST' action for a specific request. 'Accept-Encoding' can be set, but it is forbidden to put 'br' encoding in a list. 'br' encoding is not supported fro http client yet.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">To ensure, that headers are sent correctly use 'HTTP Sniffer' it is a window which automatically popups after any http request in record mode</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=httpsniffer.png');return false"> screen </a>).</div>
	<div class="tr tooltip-paragraph-last-fold">Use 'Clear Headers' action if you want to disable all headers set with this action.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
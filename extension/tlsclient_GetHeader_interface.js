<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
        id:"Value", 
        description:tr("Header Name"), 
        default_selector: "string", 
        disable_int:true, 
        help: {description: tr("Response header name. Header name must contain one or several words separated with dash. Character case is not important."), examples:[{code:"Location",description:tr("Get 'Location' header.")},{code:"Content-Type",description:tr("Get 'Content-Type' header.")},{code:"cOnTeNt-tYpE",description:tr("Also gets 'Content-Type' header.")}]}
    }) %>
    
	<%= _.template($('#variable_constructor').html())({
        id:"Save", 
        description:tr("Variable To Save"), 
        default_variable: "SAVED_HEADER", 
        help: {description: tr("A variable name, which will hold header value.")}
    }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get response header of for last request.</div>
	<div class="tr tooltip-paragraph-fold">If request was performed with redirects, only headers for last redirect will be obtained. If you want to get headers for request in a middle of redirect chain, you need to disable 'Follow redirects' option for 'GET' and 'POST' action and perform redirects by yourself. To get url of next redirect, use 'Get Header' action with 'Location' parameter.</div>
	<div class="tooltip-paragraph-last-fold"><span class="tr">Headers can be shown in browser developer tools</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=showheaders.png');return false">screen</a> ).</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
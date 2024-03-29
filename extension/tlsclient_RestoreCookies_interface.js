<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({
        id:"Url", 
        description:tr("URL"), 
        default_selector: "string", 
        disable_int:true, 
        help: {
            description: tr("Set specified cookies for a given site"), 
            examples:[
                {code:"URL",description:tr("http:// or https:// is required!\r\nURL for which to set cookies")},
            ]
       }
    }) %>
	<%= _.template($('#input_constructor').html())({
        id:"Value", 
        description:tr("Cookies"), 
        default_selector: "string", 
        disable_int:true, 
        help: {description: tr("This field must contain string with cookies in json format. That json must hold list with all cookies for all domains. Json format is described below. Use empty string to clear cookies."), examples:[{code:tr("Empty string"),description:tr("Clear cookies")}]}
    }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Set cookies for http client.</div>

	<div class="tr tooltip-paragraph-fold">'Cookies' parameter must contain string in json format with all cookies. This string can be obtained as 'HTTP-Client Save Cookies' action result and changed optionally.</div>

	<div class="tr tooltip-paragraph-fold">Most of sites uses cookies to store user data, like user authorisation. This means, that by storing cookies after authorisation on some site and restore them later you can implement autologin.</div>
	<div class="tr tooltip-paragraph-fold">Cookie values is just a string, so you may save it to file, or database and use them later.</div>
	<div class="tr tooltip-paragraph-fold">Cookies from http client may be sent to browser by using 'Load cookies from http client' action. 'Load cookies from browser' will send cookies in opposite direction.</div>
	<div class="tr tooltip-paragraph-fold">Browser cookies and http client cookies shares same format.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Cookies must be a map with a single 'cookies' key, which holds a list, for example {"cookies": [COOKIE1,COOKIE2,COOKIE3,...]}.</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=cookiesformat2.png');return false"> screen </a>).</div>

	<div class="tr tooltip-paragraph-fold">If you want to change cookies or extract them, you need to use javascript. Javascript code can be used in any field if you set field type to expression, or with 'Execute code' action.</div>

	<div class="tr tooltip-paragraph-fold">Each cookie is a map with following keys: value, name, domain, path, creation, expires, has_expires, httponly, last_access, secure.</div>
	<div class="tr tooltip-paragraph-fold">has_expires, secure and httponly are boolean values and serialized as string with values '1' or '0'.</div>
	<div class="tr tooltip-paragraph-fold">last_access, expires, creation is datetime and serialized as map with values: day_of_week, hour, millisecond, minute,month, second, year. These values are self explanatory.</div>
	<div class="tr tooltip-paragraph-fold">Here are some javascript snipsets to work with cookies. All of them may be placed into 'Execute code' action. Before using it, you must parse cookies from string like this [[COOKIES]] = JSON.parse([[SAVED_COOKIES]])["cookies"]</div>
	<div class="tr tooltip-paragraph-fold">Find cookie by name: [[COOKIE]] = [[COOKIES]].filter(function(el){return el["name"] == "REPLACE WITH COOKIE NAME"})[0]</div>
	<div class="tr tooltip-paragraph-fold">Check if cookie exists: [[COOKIE_EXISTS]] = [[COOKIES]].filter(function(el){return el["name"] == "REPLACE WITH COOKIE NAME"}).length > 0</div>
	<div class="tr tooltip-paragraph-fold">Extract cookie value [[COOKIE_VALUE]] = [[COOKIE]]["value"]</div>
	<div class="tr tooltip-paragraph-fold">You can also set cookie value [[COOKIE]]["value"] = "My value".</div>
	<div class="tooltip-paragraph-fold"><span class="tr">After you finish manipulation with [[COOKIES]] array you need to convert it to string with JSON.stringify([[COOKIES]]) code</span> (<a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=cookiesrestore2.png');return false"> screen </a>).</div>
	<div class="tr tooltip-paragraph-last-fold">[[COOKIES]] variable will hold a list of cookies, so it also may be processed with actions from 'List' module.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
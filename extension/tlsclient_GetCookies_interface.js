<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({
        id:"Save", 
        description:tr("Variable To Save"), 
        default_variable: "SAVED_COOKIES", 
        help: {description: tr("This variable will contain string with json. That json will hold list with all cookies for all domains. Json format is described below.")}
    }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Save all http client cookies into variable.</div>

	<div class="tr tooltip-paragraph-fold">Result variable will contain string with json. That json will hold list with all cookies for all domains.</div>
	<div class="tr tooltip-paragraph-fold">Most of sites uses cookies to store user data, like user authorisation. This means, that by storing cookies after authorisation on some site and restore them later you can implement autologin.</div>
	<div class="tr tooltip-paragraph-fold">Cookie values is just a string, so you may save it to file, or database and use them later.</div>
	<div class="tr tooltip-paragraph-fold">Cookies from http client may be sent to browser even without this action by using 'Load cookies from http client' action. 'Load cookies from browser' will send cookies in opposite direction.</div>
	<div class="tr tooltip-paragraph-fold">Browser cookies and http client cookies shares same format.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">Cookies string may be converted to object with JSON.parse function, like this [[JSON]] = JSON.parse([[SAVED_COOKIES]]). Result is a map with a single 'cookies' key, which holds a list, for example {"cookies": [COOKIE1,COOKIE2,COOKIE3,...]}.</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=cookiesformat2.png');return false">screen</a> ).</div>

	<div class="tr tooltip-paragraph-fold">If you want to change cookies or extract them, you need to use javascript. Javascript code can be used in any field if you set field type to expression, or with 'Execute code' action.</div>

	<div class="tr tooltip-paragraph-fold">Each cookie is a map with following keys: value, name, domain, path, creation, expires, has_expires, httponly, last_access, secure.</div>
	<div class="tr tooltip-paragraph-fold">has_expires, secure and httponly are boolean values and serialized as string with values '1' or '0'.</div>
	<div class="tr tooltip-paragraph-fold">last_access, expires, creation is datetime and serialized as map with values: day_of_week, hour, millisecond, minute,month, second, year. These values are self explanatory.</div>
	<div class="tr tooltip-paragraph-fold">Here are some javascript snipsets to work with cookies. All of them may be placed into 'Execute code' action. Before using it, you must parse cookies from string like this [[COOKIES]] = JSON.parse([[SAVED_COOKIES]])["cookies"]</div>
	<div class="tr tooltip-paragraph-fold">Find cookie by name: [[COOKIE]] = [[COOKIES]].filter(function(el){return el["name"] == "REPLACE WITH COOKIE NAME"})[0]</div>
	<div class="tr tooltip-paragraph-fold">Check if cookie exists: [[COOKIE_EXISTS]] = [[COOKIES]].filter(function(el){return el["name"] == "REPLACE WITH COOKIE NAME"}).length > 0</div>
	<div class="tr tooltip-paragraph-fold">Extract cookie value [[COOKIE_VALUE]] = [[COOKIE]]["value"]</div>
	<div class="tr tooltip-paragraph-fold">You can also set cookie value [[COOKIE]]["value"] = "My value". But changing parsed object won't change cookie inside http client, to do that you need to convert cookie to string(with JSON.stringify([[COOKIES]])) and use it with 'HTTP-Client Restore Cookies' action.</div>
	<div class="tr tooltip-paragraph-last-fold">[[COOKIES]] variable will hold a list of cookies, so it also may be processed with actions from 'List' module.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
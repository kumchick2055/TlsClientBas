<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({id:"Save", description:tr("Variable To Save"), default_variable: "SAVED_CONTENT", help: {description: tr("This variable will contain responce text without headers after action will end.")}}) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get page source of the last request of http client.</div>
	<div class="tr tooltip-paragraph-fold">Page source is the same as one, which is displayed after you hit CTRL-U in a browser.</div>
	<div class="tr tooltip-paragraph-fold">The responce doesn't contain headers, to get headers you need to use 'Get Header' action.</div>
	<div class="tr tooltip-paragraph-fold">You need to set 'base64' encoding if you are getting binary data. Many actions accept data in base64 format.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">You can specify exact encoding name, see</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('http://doc.qt.io/qt-5/qtextcodec.html#details');return false">list of suported encodings</a>.</div>
	
	<div class="tr tooltip-paragraph-last-fold">The fastest way to extract data from page source is xpath. 'Http Client' has set of actions to perform xpath queries. It is not recomended to use 'Content' action and regular expressions unless it is absolutely necessary.</div>
</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>

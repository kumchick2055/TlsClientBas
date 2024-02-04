<div class="container-fluid">
	<%= _.template($('#variable_constructor').html())({
        id:"Save", 
        description:tr("Variable To Save"), 
        default_variable: "SAVED_STATUS", 
        help: {description: tr("Variable, which will contain http status as integer."), examples:[{code:"200",description:tr("The request has succeeded.")},{code:"302",description:tr("The target page resides under different url.")},{code:"404",description:tr("Page not found")}]}
    }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get status of the last request of http client.</div>
	<div class="tr tooltip-paragraph-fold">You can compare status with 'If' action like this [[SAVED_STATUS]] == 200. This expression checks if request was successful.</div>
	
	<div class="tooltip-paragraph-last-fold"><span class="tr">List of</span> <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://httpstatuses.com/');return false">http statuses</a>.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
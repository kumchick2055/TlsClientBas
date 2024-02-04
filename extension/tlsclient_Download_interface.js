<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
        id:"Value", 
        description:tr("Url"), 
        default_selector: "string", 
        disable_int:true, 
        help: {description: tr("Request url"), examples:[{code:"https://google.com",description:tr("Url with scheme")},{code:"google.com",description:tr("Url without scheme")},{code:"https://www.google.com/search?q=cats",description:tr("Url with query string")},{code:"https://www.google.com/search?q=%D0%BA%D0%BE%D1%82%D1%8B",description:tr("Url with encoded query. To encode string use encodeURIComponent function.")}]}
    }) %>
    
    <%= _.template($('#input_constructor').html())({
        id:"File", 
        description:tr("File Path"), 
        default_selector: "string", 
        disable_int:true, 
        help: {description: tr("A path to file which will be downloaded. If file exists, it will be overwritten. Putting here resource with type 'LinesFromFile' is almost always a bad idea, because resource will be converted to line from selected file, not to file name. If you want to get file name from resource, please use 'Resource Location' action and put result here."), examples:[{code:"c:/downloads/pic.png"},{code:"c:\\downloads\\pic.png"}]}
    }) %>
    
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Download selected url and save response to file.</div>
	<div class="tr tooltip-paragraph-fold">If you don't want to save data on disk, you should use 'GET' or 'POST' action.</div>
	<div class="tr tooltip-paragraph-fold">This action uses cookies and headers just like 'GET' and 'POST' actions.</div>
	<div class="tr tooltip-paragraph-fold">Special characters and non-latin query parts must be encoded with encodeURIComponent function. For example, instead of using https://www.google.com/search?q=коты, url must be https://www.google.com/search?q=%D0%BA%D0%BE%D1%82%D1%8B</div>
	<div class="tr tooltip-paragraph-fold">HTTP headers(like User-Agent) can be specified with 'HTTP-Client Set Header' action.</div>
	<div class="tr tooltip-paragraph-fold">'Download' action will not set any variable, to obtain responce you need to call 'Current Url', 'Status' or 'Get Header' after request is performed. And, of course, specified file will contain responce text.</div>
	<div class="tr tooltip-paragraph-fold">'Current Url' will get actual url after all redirects.</div>
	<div class="tr tooltip-paragraph-fold">'Status' returns HTTP status, for example, 200 - if request is performed correctly, 500 - in case of server error, etc.</div>
	<div class="tr tooltip-paragraph-fold">'Get Header' returns responce header, like 'Content-Type', 'Date', 'Set-Cookie'</div>
	<div class="tr tooltip-paragraph-last-fold">You can set max page loading time by clicking on hourglass icon near cancel button.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
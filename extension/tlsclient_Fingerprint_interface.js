<div class="container-fluid">
    <%= _.template($("#combobox").html())({ id: "fingerprint", title: tr("Fingerprint"), search: true, items: [
        {value: "chrome_103", name: "Chrome 103", title: "", selected: true},
        {value: "chrome_104", name: "Chrome 104", title: ""},
        {value: "chrome_105", name: "Chrome 105", title: ""},
        {value: "chrome_106", name: "Chrome 106", title: ""},
        {value: "chrome_107", name: "Chrome 107", title: ""},
        {value: "chrome_108", name: "Chrome 108", title: ""},
        {value: "chrome109", name: "Chrome 109", title: ""},
        {value: "Chrome110", name: "Chrome 110", title: ""},
        {value: "chrome111", name: "Chrome 111", title: ""},
        {value: "chrome112", name: "Chrome 112", title: ""},
        {value: "firefox_102", name: "Firefox 102", title: ""},
        {value: "firefox_104", name: "Firefox 104", title: ""},
        {value: "firefox108", name: "Firefox 108", title: ""},
        {value: "Firefox110", name: "Firefox 110", title: ""}
        
    ]    
    }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Выберите Браузер отпечаток которого вы хотите установить.</div>
</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
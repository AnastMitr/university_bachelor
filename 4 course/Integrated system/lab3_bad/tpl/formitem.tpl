<form action="{selfurl}" method=GET>
<input type=hidden name=id value={id}>
<input type=hidden name=fr value={fr}>
<input type=hidden name=type value='add'>

To: <input type=text name=to value="{to}"><br>
Message: <textarea rows=10 cols=60 wrap=virtual name=message>{message}</textarea><br>

<input type=submit value="Ok">

</form>
$pattern = "\s+[A-Z0-9]+\s+[A-Z0-9]+\s+[A-Z0-9]{8}";
$platform = "x86";

if( $args.length -gt 0 )
{
    if($args[1])
    {
        $platform = $args[1];
    } 
    Write-Host ("platform");
    Write-Host ($platform);
    Write-Host ("");

    $dll = [System.IO.Path]::GetFilename($args[0]);
    $def = [System.IO.Path]::ChangeExtension($dll, "def");
    $lib = [System.IO.Path]::ChangeExtension($dll, "lib");

    Write-Host ("Generating " + $def + " file...");
    "EXPORTS" > $def;
    &"dumpbin" "/exports" $args[0] | select-string $pattern | %{$null = $_.Line -match $pattern; ("`t" + $matches['symbol']) >> $def; }

    Write-Host ("Generating " + $lib + " file...");
    &"lib" ("/def:" + $def) ("/out:" + $lib) ("/machine:" + $platform) | out-null;

    Write-Host ("succsess "+$PSScriptRoot+"\"+$lib);
}
else
{
    Write-Host "Start powershell from VisualStudio commandline then use this script.";
    Write-Host "Script takes two parameters: dll filename and platform.";
    Write-Host "example: .\GenerateLibFromDll.ps1 hello.dll `"x64`"";
    Write-Host "or to process all dlls in the dir: gci *.dll | foreach {&`".\GenerateLibFromDll.ps1`" $_.Name `"x64`"}";;
}
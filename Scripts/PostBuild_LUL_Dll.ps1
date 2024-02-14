param (
    [string]$dll = $(throw "-dll is required."),
    [string]$src = $(throw "-src is required."),
    [string]$dst = $(throw "-dst is required.")
)

#write-output "$dll"
#write-output "$src"
#write-output "$dst"
#
#write-output "---------"

$dirNameList =  (Get-ChildItem $dst -Directory -Recurse) -replace '\..*$'

foreach ($dirName in $dirNameList)
{
    write-output "Copy $dll to $dst\$dirName\$dll"
    Copy-Item $src -Destination $dst\$dirName\$dll.dll -Force
}

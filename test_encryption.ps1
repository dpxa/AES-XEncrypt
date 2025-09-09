$Folder1 = ".\test_files\encrypt_test"
$Folder2 = ".\test_files\expected"

function Get-FileHashTable {
    param ($FolderPath)
    $hashTable = @{}

    Get-ChildItem -Path $FolderPath -Recurse -File | ForEach-Object {
        $relativePath = $_.FullName.Substring((Get-Item $FolderPath).FullName.Length).TrimStart("\")
        $hash = Get-FileHash $_.FullName -Algorithm SHA256
        
        if ($hash) {
            $hashTable[$relativePath] = $hash.Hash
        }
    }
    
    return $hashTable
}

$HashTable1 = Get-FileHashTable -FolderPath $Folder1
$HashTable2 = Get-FileHashTable -FolderPath $Folder2

$allKeys = $HashTable1.Keys + $HashTable2.Keys | Select-Object -Unique
$discrepancies = @()

$count = 0
foreach ($key in $allKeys) {
    if (-not $HashTable1.ContainsKey($key)) {
        $count++;
        $discrepancies += "$count Missing in folder 1: $Folder1\$key"
    } elseif (-not $HashTable2.ContainsKey($key)) {
        $count++;
        $discrepancies += "$count Missing in folder 2: $Folder2\$key"
    } elseif ($HashTable1[$key] -ne $HashTable2[$key]) {
        $count++;
        $discrepancies += "$count File mismatch: $key"
    }
}

if ($discrepancies.Count -eq 0) {
    Write-Host "Folders are identical"  -ForegroundColor Green
} else {
    Write-Host "Discrepancies found"  -ForegroundColor Red
    $discrepancies | ForEach-Object { Write-Host $_ -ForegroundColor Yellow }
}
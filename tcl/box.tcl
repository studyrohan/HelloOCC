
# Load Modeling algorithms.
pload MODELING

# Make the box.
box theBox 1.0 2.0 3.0
set boxInfo [dump theBox]

# Save box BRep info to file.
if [catch {open box.txt w+} theFile] {
    puts "Cannot open d:/box.txt for writing: $theFile"
} else {
    puts $theFile $boxInfo
    flush $theFile
    close $theFile
}

puts "Dump Info Finished!"
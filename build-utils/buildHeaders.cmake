
# Generate some MB-System header files 
## message("Now in buildHeaders")
### buildHeaders("xyz")

function(buildHeaders otpsDir levitusDir)

   message("otpsDir: ${otpsDir}, levitusDir: ${levitusDir}")
   message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
   execute_process(COMMAND "echo" "// This file is automatically generated"
		OUTPUT_FILE "/tmp/part1"
		)

   execute_process(COMMAND "echo" "const char *otps_location = \"${otpsDir}\";"
		OUTPUT_FILE "/tmp/part2"
		)

  execute_process(COMMAND "cat" "/tmp/part1" "/tmp/part2"
		OUTPUT_FILE ${CMAKE_SOURCE_DIR}/src/otps/otps.h)  


   execute_process(COMMAND "echo" "// This file is automatically generated"
		OUTPUT_FILE "/tmp/part1"
		)

  execute_process(COMMAND "echo" "const char *levitusfile = \"${levitusDir}/LevitusAnnual82.dat\";"
		OUTPUT_FILE "/tmp/part2"
		)

   execute_process(COMMAND "echo" "const char *otps_location = \"${otpsDir}\";"
		OUTPUT_FILE "/tmp/part3"
		)
		
  execute_process(COMMAND "cat" "/tmp/part1" "/tmp/part2" "/tmp/part3"
		OUTPUT_FILE ${CMAKE_SOURCE_DIR}/src/utilities/levitus.h)
		
endfunction()


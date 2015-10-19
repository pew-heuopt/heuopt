#!/usr/bin/python


import subprocess, re

input_dir= "instances/"
input_instances= [

                   'automatic-1.txt', 
                   'automatic-2.txt',
                   'automatic-3.txt',
                   'automatic-4.txt',
                   'automatic-5.txt',

                   'automatic-6.txt',
                   'automatic-7.txt',
                   'automatic-8.txt',
                   'automatic-9.txt',
                   'automatic-10.txt',
                   ]

output_dir= "output/"

program_instances= { 

                     'cnstr-asc' : { 'bin' : 'src/assignment1_1', 
                                     'opt' : [ '--spine-order', 'ascend'] },

                     'cnstr-sorted' : { 'bin' : 'src/assignment1_1', 
                                        'opt' : [ '--spine-order', 'sorted'] },
                   }
     


if __name__ == '__main__' :

    results= {};


    # execute programs
    for program_name in program_instances:

        program_instance= program_instances[program_name]

        results[program_name]= {}

        for input_instance in input_instances :

            results[program_name][input_instance]= {}


            proc = subprocess.Popen([ program_instance['bin'], 
                                      "--input", input_dir + input_instance, 
                                      "--output",  output_dir + input_instance + '.be',
                                       ] + program_instance['opt'], 
                                     
                                     stdout=subprocess.PIPE)
            for line in proc.stdout:
                str= line.rstrip();
                match= re.search("crossings sum: (\d+)", str)
                if match:
                    num= match.group(1);
                    results[program_name][input_instance]['crossings']= num
            #        print program_name, " crossings: ", num



    crossings_stat_file = open( output_dir + 'crossings_stat.data', 'w') 

    header_line= "instance";
    for program_name in program_instances:
        header_line= header_line + " " + program_name ;
    crossings_stat_file.write( header_line + "\n" )

    for input_instance in input_instances :
        result_line= input_instance;
        for program_name in program_instances:
            result_line = result_line + " " + results[program_name][input_instance]['crossings']
        crossings_stat_file.write( result_line + "\n" )

        

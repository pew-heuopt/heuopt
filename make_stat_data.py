#!/usr/bin/python


import os, subprocess, re

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

                     'cnstr-component' : { 'bin' : 'src/assignment1_1', 
                                        'opt' : [ '--spine-order', 'component'] },


                     'cnstr-random' : { 'bin' : 'src/assignment1_2', 
                                        'opt' : [ ] },


                   }
     


def write_statfile( program_instances, results, filename, instance_property ) :

    stat_file = open( output_dir + filename, 'w') 

    header_line= "instance"
    for program_name in program_instances:
        header_line= header_line + " " + program_name ;

    stat_file.write( header_line + "\n" )

    for input_instance in input_instances :
        result_line= input_instance;
        for program_name in program_instances:
            result_line = result_line + " " + results[program_name][input_instance][instance_property]
        stat_file.write( result_line + "\n" )



if __name__ == '__main__' :

    results= {};

    if not os.path.exists( output_dir):
        os.makedirs(output_dir)

    # execute programs
    for program_name in program_instances:

        program_instance= program_instances[program_name]

        results[program_name]= {}

        for input_instance in input_instances :

            results[program_name][input_instance]= {}


            proc = subprocess.Popen([ "/usr/bin/time", 
                                      "-f", "time sum: user %U sys %S",
                                      program_instance['bin'], 
                                      "--input", input_dir + input_instance, 
                                      "--output",  output_dir + input_instance + '.be',
                                       ] + program_instance['opt'], 
                                     
                                     stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
            for line in proc.stdout:

                linestr= line.rstrip();

                match= re.search("crossings sum: (\d+)", linestr)
                if match:
                    num= match.group(1);
                    results[program_name][input_instance]['crossings']= num
                    continue

                match= re.search("time sum: user (\d+.?\d*) sys (\d+.?\d*)", linestr)
                if match:
                    user= float( match.group(1) );
                    sys= float( match.group(2) );
                    results[program_name][input_instance]['time']= str( user + sys );
                    continue


    write_statfile( program_instances, results, "crossings_stat.data", "crossings" )
    write_statfile( program_instances, results, "time_stat.data", "time" )



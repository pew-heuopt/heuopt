#!/usr/bin/python



import os, subprocess, re

input_dir= "instances/"
input_instances= [
                   'automatic-1.txt',
                   'automatic-2.txt',
                   'automatic-3.txt',
                   'automatic-4.txt',
                   'automatic-5.txt',
#                   'automatic-6.txt',
#                   'automatic-7.txt',
#                   'automatic-8.txt',
#                   'automatic-9.txt',
#                   'automatic-10.txt',
                   ]

output_dir= "output/"

program_instances= { 
                     'lsr-1node-first' : { 'bin' : 'src/assignment2', 
                                               'opt' : [  '--neighborhood', '1-node',
                                                          '--step-func', 'first'] },

                     'lsr-1node-best' : { 'bin' : 'src/assignment2', 
                                              'opt' : [ '--neighborhood', '1-node',
                                                        '--step-func', 'best'] },

                     'lsr-1node-rand' : { 'bin' : 'src/assignment2', 
                                               'opt' : [ '--neighborhood', '1-node',
                                                         '--step-func', 'random'] },



                     'lsr-1edge-first' : { 'bin' : 'src/assignment2', 
                                               'opt' : [  '--neighborhood', '1-edge',
                                                          '--step-func', 'first'] },

                     'lsr-1edge-best' : { 'bin' : 'src/assignment2', 
                                              'opt' : [ '--neighborhood', '1-edge',
                                                        '--step-func', 'best'] },

                     'lsr-1edge-rand' : { 'bin' : 'src/assignment2', 
                                               'opt' : [ '--neighborhood', '1-edge',
                                                         '--step-func', 'random'] },



                     'lsr-1node-edge-first' : { 'bin' : 'src/assignment2', 
                                               'opt' : [  '--neighborhood', '1-node-edge',
                                                          '--step-func', 'first'] },

                     'lsr-1node-edge-best' : { 'bin' : 'src/assignment2', 
                                              'opt' : [ '--neighborhood', '1-node-edge',
                                                        '--step-func', 'best'] },

                     'lsr-1node-edge-rand' : { 'bin' : 'src/assignment2', 
                                               'opt' : [ '--neighborhood', '1-node-edge',
                                                         '--step-func', 'random'] },

#
# assignament 1
#

#                     'cnstr-asc' : { 'bin' : 'src/assignment1_1', 
#                                     'opt' : [ '--spine-order', 'ascend'] },
#
#                     'cnstr-sorted' : { 'bin' : 'src/assignment1_1', 
#                                        'opt' : [ '--spine-order', 'sorted'] },
#
#                     'cnstr-component' : { 'bin' : 'src/assignment1_1', 
#                                        'opt' : [ '--spine-order', 'component'] },
#
#
#                     'cnstr-random' : { 'bin' : 'src/assignment1_2', 
#                                        'opt' : [ ] },
#

                   }
     


def write_statfile( program_instances, results, filename, instance_property ) :

    stat_file = open( output_dir + filename, 'w') 

    header_line= "instance"
    for program_name in program_instances:
        header_line= header_line + " " + program_name ;

    stat_file.write( header_line + "\n" )

    for input_instance in input_instances :
        result_line= input_instance;
        for program_name in sorted( program_instances, key=program_instances.get) :
            if( instance_property in results[program_name][input_instance] ) :
                val= results[program_name][input_instance][instance_property]
            else :
                val= 0
            result_line = result_line + " " + str(val)
        stat_file.write( result_line + "\n" )



def execute_program( program_instance, input_instance, filename ) :

    """ 
    executes program and returns statistic information in a dictionary.

    arguments:

        program_name -- a key of the global dictionary 'program_instance'

    return: 
    
        dictionary containing statistic information 
        
              { time :      runtime in seconds (user and sys, not wall clock)
                crossings : number of crossings
              }
    """

    results= {}


    proc = subprocess.Popen([ "/usr/bin/time", 
                              "-f", "time sum: user %U sys %S",
                              program_instance['bin'], 
                              "--input", input_dir + input_instance, 
                              "--output", filename,
                               ] + program_instance['opt'], 
                             
                             stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
    for line in proc.stdout:

        linestr= line.rstrip();

        match= re.search("stagnation: (\d+)", linestr)
        if match:
            num= match.group(1);
            results['stagnation']= int(num)
            continue


        match= re.search("crossings sum: (\d+)", linestr)
        if match:
            num= match.group(1);
            results['crossings']= int(num)
            continue

        match= re.search("time sum: user (\d+.?\d*) sys (\d+.?\d*)", linestr)
        if match:
            user= float( match.group(1) );
            sys= float( match.group(2) );
            results['time']= user + sys;
            continue


    return results



if __name__ == '__main__' :

    results= {};

    if not os.path.exists( output_dir):
        os.makedirs(output_dir)

    # execute programs
    for program_name in program_instances:

        program_instance= program_instances[program_name]

        results[program_name]= {}

        for input_instance in input_instances :

            results[program_name][input_instance]= execute_program( program_instance, input_instance,  output_dir + program_name + "_" +input_instance + '.be');

    write_statfile( program_instances, results, "stagnation_stat.data", "stagnation" )
    write_statfile( program_instances, results, "timeout_stat.data", "timeout" )
    write_statfile( program_instances, results, "crossings_stat.data", "crossings" )
    write_statfile( program_instances, results, "time_stat.data", "time" )



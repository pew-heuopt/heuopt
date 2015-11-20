#!/usr/bin/python 


import sys, getopt, math, make_stat_data


NUM_RUNS= 30

PROGRAM_PROPERTIES= [ 'crossings' , 'time' ]



def usage() :

    print "make_random_stat.py <--program name>"


def make_stat( results, param_key ) :

        param_sum = 0.0
        param_num= len(results)

        for r in results :
            param_sum += r[param_key]

        avg= param_sum / param_num;


        var= 0.0;
        for r in results :
            var += ( r[param_key] - avg ) ** 2
#            print( str(r[param_key]) +" - " + str(avg) +  "var: " + str(var))

        var /= param_num;

        return  { 'var' : var, 
                  'avg' : avg,
                  'sd' : math.sqrt(var) };


def write_random_statfile( results, filename, program_property ) :

    stat_file = open( make_stat_data.output_dir + filename, 'w') 

    stat_params= [ 'var', 'avg', 'sd', 'min', 'min_run' ]

    header_line= "instance"
    for stat_param in stat_params :
        header_line= header_line + " " + stat_param ;

    stat_file.write( header_line + "\n" )

    for input_instance in make_stat_data.input_instances :
        result_line= input_instance;
        for stat_param in stat_params :
            result_line = result_line + " " + str(results[input_instance][program_property][stat_param])
        stat_file.write( result_line + "\n" )




def main(argv):                         

    program = None 

    try:                             
        opts, args = getopt.getopt(argv, "p", ["help", "program="]) 
    except getopt.GetoptError:
        usage()
        sys.exit(2)                     

    for opt, arg in opts:

        if opt in ("-h", "--help"):
            usage()
            sys.exit()

        elif opt in ("-p", "--program"):
    
            program = arg   

    if( not program ) :

        print("no program specified")
        usage()
        sys.exit(1)


    program_instance= None 

    try :

        program_instance= make_stat_data.program_instances[program]
            
    except KeyError :

        print("no such program specified")

        for key in make_stat_data.program_instances :
            print( "    " + key )

        sys.exit(1)


    stat_results= {}

    for input_instance in make_stat_data.input_instances :

        print input_instance 
        stat_results[input_instance]= {}
        program_results= []

        param_min= {}
        min_run= {}

        for program_property in  PROGRAM_PROPERTIES :

            param_min[program_property]= -1
            min_run[program_property]= -1

        for i in range (0,(NUM_RUNS-1)) :

            res= make_stat_data.execute_program( program_instance, input_instance,  make_stat_data.output_dir + input_instance + '_' + str(i) + '.be' )
            program_results.append( res  )

            for program_property in  PROGRAM_PROPERTIES :

                if( ( param_min[program_property] == -1 ) or
                    ( res[program_property] < param_min[program_property] ) ) :

                    param_min[program_property] = res[program_property] 
                    min_run[program_property] = i



        for program_property in  PROGRAM_PROPERTIES :

            stat_results[input_instance][program_property]= make_stat( program_results, program_property )
            stat_results[input_instance][program_property]['min']=  param_min[ program_property]
            stat_results[input_instance][program_property]['min_run']= min_run[ program_property]

    for program_property in  PROGRAM_PROPERTIES :
        write_random_statfile( stat_results, program_property + "_" + program + ".data" , program_property ) 


if __name__ == '__main__' :

    main(sys.argv[1:])

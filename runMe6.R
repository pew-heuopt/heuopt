COMMAND <- "src/assignment4"
SEL_INSTANCE <- "instances/automatic-1.txt"


##' @param script 
##' @return 
##' @author Alexander
runMe <- function(nAnts,nRuns,instance,alpha,beta,evaporation,useDaemon,script,n,x=NULL) {
    res <- numeric(n)
    timings <- numeric(n)
    for(i in 1:n) {
        outFileString <- paste("output/aco6_out_",gsub("/","_",instance),x,n,".txt",sep="")
        commandString <- paste(script,"--num-ants",nAnts,"--num-runs",nRuns,"--beta",beta,"--alpha",alpha,
                             "--evaporation",evaporation,"--use-daemon",useDaemon, "--input",instance,"--output",outFileString)
        #print(commandString)
        timing <- system.time(out <- system(commandString,intern=TRUE))
        crossingSum <- tail(out,1)
        ## regexp would be nice, but so far, stringsplit at " " and take the third element of the resulting vector
        res[i] <- as.numeric(strsplit(crossingSum," ")[[1]][3])
        timings[i] <- timing["elapsed"]
    }
    if(n==1) {
        return(list(res=res,avg=mean(res),timings=timings,avgTime=mean(timings)))
    } else {
        return(list(res=res,avg=mean(res),stdDevs=sd(res),timings=timings,avgTime=mean(timings)))
    }
} 

##' Parallel version of runMe
##'
##' @title Parallel version of runMe
##' @param x identifier of this instance
##' @param params parameter matrix
##' @param instance which graph instance we should use
##' @param script C++ program to run
##' @param n number of runs
##' @return 
##' @author Alexander
pRunMe <- function(x,params,instance,script=COMMAND,n=20) {
    alpha <- params[x,"alpha"]
    beta <- params[x,"beta"]
    nRuns <- params[x,"nRuns"]
    nAnts <- params[x,"nAnts"]
    useLocalSearch <- params[x,"useLocalSearch"]
    n <- params[x,"n"]
    res <- runMe(nAnts=nAnts,nRuns=nRuns,instance=instance,alpha=alpha,beta=beta,script=script,n=n,
                 useLocalSearch=useLocalSearch)
    return(res)
}



library(parallel)



## alpha <- c(0.1,1,2)
## beta <- c(0.1,1,2)
## nRuns <- c(5,10,20,50,100)
## nAnts <- c(100,500,1000,2000)
## n <- 10
## params <- expand.grid(alpha,beta,nRuns,nAnts,n)
## colnames(params) <- c("alpha","beta","nRuns","nAnts","n")


## optim_res <- mclapply(1:nrow(params),pRunMe,params=params,instance=SEL_INSTANCE,mc.cores=8)

## ## create object which will be saved
## opt_res <- list(optim_res=optim_res,params=params)

## optResFileString <- paste("output/aco_opt_",gsub("/","_",SEL_INSTANCE),".Rdata",sep="")
## save(opt_res,file=optResFileString)



runMeWrapper <- function(x,nAnts,nRuns,instance,alpha,beta,evaporation=0.4,useDaemon=0,script) {
    res1 <- runMe(nAnts=nAnts,nRuns=nRuns,instance=instance,alpha=alpha,beta=beta,script=script,n=1,x=x,
                  evaporation=evaporation,useDaemon=useDaemon)
    return(res1)
}


res1old <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=50,instance="instances/automatic-1.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res1tuned <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=50,instance="instances/automatic-1.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)

save(res1old,file="output/tuning/res61old.rdata")
save(res1tuned,file="output/tuning/res61tuned.rdata")

res2old <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=50,instance="instances/automatic-2.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res2tuned <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=50,instance="instances/automatic-2.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)

save(res2old,file="output/tuning/res62old.rdata")
save(res2tuned,file="output/tuning/res62tuned.rdata")

res3old <- mclapply(1:8,runMeWrapper,nAnts=20000,nRuns=40,instance="instances/automatic-3.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res3tuned <- mclapply(1:8,runMeWrapper,nAnts=20000,nRuns=40,instance="instances/automatic-3.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)
save(res3old,file="output/tuning/res53.rdata")
save(res3tuned,file="output/tuning/res53nl.rdata")

res4old <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=40,instance="instances/automatic-4.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res4tund <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=40,instance="instances/automatic-4.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)
save(res4old,file="output/tuning/res54.rdata")
save(res4tuned,file="output/tuning/res54nl.rdata")


res5old <- mclapply(1:8,runMeWrapper,nAnts=5000,nRuns=40,instance="instances/automatic-5.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res5tuned <- mclapply(1:8,runMeWrapper,nAnts=5000,nRuns=40,instance="instances/automatic-5.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)
save(res5old,file="output/tuning/res55.rdata")
save(res5tuned,file="output/tuning/res55nl.rdata")

res6old <- mclapply(1:8,runMeWrapper,nAnts=10,nRuns=2,instance="instances/automatic-6.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res6tuned <- mclapply(1:8,runMeWrapper,nAnts=10,nRuns=2,instance="instances/automatic-6.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)
save(res6old,file="output/tuning/res56.rdata")
save(res6tuned,file="output/tuning/res56.rdata")

res7old <- mclapply(1:8,runMeWrapper,nAnts=200,nRuns=20,instance="instances/automatic-7.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res7tuned <- mclapply(1:8,runMeWrapper,nAnts=200,nRuns=20,instance="instances/automatic-7.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)
save(res7old,file="output/tuning/res57.rdata")
save(res7tuned,file="output/tuning/res57nl.rdata")

res8old <- mclapply(1:8,runMeWrapper,nAnts=100,nRuns=5,instance="instances/automatic-8.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res8tuned <- mclapply(1:8,runMeWrapper,nAnts=100,nRuns=5,instance="instances/automatic-8.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)
save(res8old,file="output/tuning/res58.rdata")
save(res8tuned,file="output/tuning/res58nl.rdata")

res9old <- mclapply(1:8,runMeWrapper,nAnts=20,nRuns=5,instance="instances/automatic-9.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res9tuned <- mclapply(1:8,runMeWrapper,nAnts=20,nRuns=5,instance="instances/automatic-9.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)
save(res9old,file="output/tuning/res59.rdata")
save(res9tuned,file="output/tuning/res59nl.rdata")

res10old <- mclapply(1:8,runMeWrapper,nAnts=100,nRuns=5,instance="instances/automatic-10.txt",alpha=2,beta=2,script=COMMAND,mc.cores=8)
res10tuned <- mclapply(1:8,runMeWrapper,nAnts=100,nRuns=5,instance="instances/automatic-10.txt",alpha=0.49,beta=2.28,evaporation=0.38,useDaemon=1,script=COMMAND,mc.cores=8)
save(res10old,file="output/tuning/res510.rdata")
save(res10tuned,file="output/tuning/res510nl.rdata")

analyseResult <- function(res) {
    n <- length(res)
    crossings <- numeric(n)
    timings <- numeric(n)
    for(i in 1:n) {
        crossings[i] <- res[[i]]$res
        timings[i] <- res[[i]]$timings
    }
    minCrossing <- min(crossings)
    avgCrossing <- mean(crossings)
    sdCrossing <- sd(crossings)
    whichMinCrossing <- which(crossings==minCrossing)
    avgTime <- mean(timings)
    sdTime <- sd(timings,na.rm=TRUE)
    
    return(list(
        minCrossing=minCrossing,
        avgCrossing=avgCrossing,
        sdCrossing=sdCrossing,
        whichMinCrossing=whichMinCrossing,
        avgTime=avgTime,
        sdTime=sdTime))
}


analyseResult(res1old)
analyseResult(res1tuned)

analyseResult(res2old)
analyseResult(res2tuned)

analyseResult(res3old)
analyseResult(res3tuned)

analyseResult(res4old)
analyseResult(res4tuned)

analyseResult(res5old)
analyseResult(res5tuned)

analyseResult(res6old)
analyseResult(res6tuned)

analyseResult(res7old)
analyseResult(res7tuned)

analyseResult(res8old)
analyseResult(res8tuned)

analyseResult(res9old)
analyseResult(res9tuned)

analyseResult(res10old)
analyseResult(res10tuned)

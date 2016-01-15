COMMAND <- "src/assignment5"
SEL_INSTANCE <- "instances/automatic-1.txt"


##' @param script 
##' @return 
##' @author Alexander
runMe <- function(nAnts,nRuns,instance,alpha,beta,localSearch,script,n,x=NULL) {
    res <- numeric(n)
    timings <- numeric(n)
    for(i in 1:n) {
        outFileString <- paste("output/aco_out_",gsub("/","_",instance),x,n,".txt",sep="")
        commandString <- paste(script,"--num-ants",nAnts,"--num-runs",nRuns,"--beta",beta,"--alpha",alpha,
                              "--use-local-search",localSearch, "--input",instance,"--output",outFileString)
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



runMeWrapper <- function(x,nAnts,nRuns,instance,alpha,beta,localSearch,script) {
    res1 <- runMe(nAnts=nAnts,nRuns=nRuns,instance=instance,alpha=alpha,beta=beta,script=script,n=1,x=x,
                  localSearch=localSearch)
    return(res1)
}



res1 <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=50,instance="instances/automatic-1.txt",alpha=1,beta=1,script=COMMAND,localSearch=1,mc.cores=8)
res1nl <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=50,instance="instances/automatic-1.txt",alpha=1,beta=1,script=COMMAND,localSearch=0,mc.cores=8)

save(res1,file="output/res51.rdata")
save(res1nl,file="output/res51nl.rdata")

res2 <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=50,instance="instances/automatic-2.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=1)
res2nl <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=50,instance="instances/automatic-2.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=0)
save(res2,file="output/res52.rdata")
save(res2nl,file="output/res52nl.rdata")

res3 <- mclapply(1:8,runMeWrapper,nAnts=20000,nRuns=40,instance="instances/automatic-3.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=1)
res3nl <- mclapply(1:8,runMeWrapper,nAnts=20000,nRuns=40,instance="instances/automatic-3.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=0)
save(res3,file="output/res53.rdata")
save(res3nl,file="output/res53nl.rdata")

res4 <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=40,instance="instances/automatic-4.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=1)
res4nl <- mclapply(1:8,runMeWrapper,nAnts=10000,nRuns=40,instance="instances/automatic-4.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=0)
save(res4,file="output/res54.rdata")
save(res4nl,file="output/res54nl.rdata")


res5 <- mclapply(1:8,runMeWrapper,nAnts=5000,nRuns=40,instance="instances/automatic-5.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=1)
res5nl <- mclapply(1:8,runMeWrapper,nAnts=5000,nRuns=40,instance="instances/automatic-5.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=0)
save(res5,file="output/res55.rdata")
save(res5nl,file="output/res55nl.rdata")

res6 <- mclapply(1:8,runMeWrapper,nAnts=10,nRuns=2,instance="instances/automatic-6.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=1)
res6nl <- mclapply(1:8,runMeWrapper,nAnts=10,nRuns=2,instance="instances/automatic-6.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=0)
save(res6,file="output/res56.rdata")
save(res6,file="output/res56.rdata")

res7 <- mclapply(1:8,runMeWrapper,nAnts=200,nRuns=20,instance="instances/automatic-7.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=1)
res7nl <- mclapply(1:8,runMeWrapper,nAnts=200,nRuns=20,instance="instances/automatic-7.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=0)
save(res7,file="output/res57.rdata")
save(res7nl,file="output/res57nl.rdata")

res8 <- mclapply(1:8,runMeWrapper,nAnts=100,nRuns=5,instance="instances/automatic-8.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=1)
res8nl <- mclapply(1:8,runMeWrapper,nAnts=100,nRuns=5,instance="instances/automatic-8.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=0)
save(res8,file="output/res58.rdata")
save(res8nl,file="output/res58nl.rdata")

res9 <- mclapply(1:8,runMeWrapper,nAnts=20,nRuns=5,instance="instances/automatic-9.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=1)
res9nl <- mclapply(1:8,runMeWrapper,nAnts=20,nRuns=5,instance="instances/automatic-9.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=0)
save(res9,file="output/res59.rdata")
save(res9nl,file="output/res59nl.rdata")

res10 <- mclapply(1:8,runMeWrapper,nAnts=100,nRuns=5,instance="instances/automatic-10.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=1)
res10nl <- mclapply(1:8,runMeWrapper,nAnts=100,nRuns=5,instance="instances/automatic-10.txt",alpha=1,beta=1,script=COMMAND,mc.cores=8,localSearch=0)
save(res10,file="output/res510.rdata")
save(res10nl,file="output/res510nl.rdata")

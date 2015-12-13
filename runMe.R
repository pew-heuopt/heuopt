COMMAND <- "src/assignment4"
SEL_INSTANCE <- "instances/automatic-4.txt"

##' execute C program and collect number of crossings
##'
##' This script executes one run of our C-program. It uses system() and also creates
##' the command string 
##' @title 
##' @param nAnts number of ants
##' @param nRuns number of runs
##' @param instance instance file
##' @param outFile output file
##' @param alpha 
##' @param beta 
##' @param script 
##' @return 
##' @author Alexander
runMe <- function(nAnts,nRuns,instance,outFile,alpha,beta,script,n) {
    res <- numeric(n)
    for(i in 1:n) {
        outFileString <- paste("output/aco_out_",gsub("/","_",instance),n,".txt",sep="")
        commandString <- paste(script,"--num-ants",nAnts,"--num-runs",nRuns,"--beta",beta,"--alpha",alpha,
                               "--input",instance,"--output",outFileString)
        out <- system(commandString,intern=TRUE)
        crossingSum <- tail(out,1)
        ## regexp would be nice, but so far, stringsplit at " " and take the third element of the resulting vector
        res[i] <- as.numeric(strsplit(crossingSum," ")[[1]][3])
    }
    
    return(list(res=res,avg=mean(res),stdDevs=sd(res)))
}

##' Parallel version of runMe
##'
##' @title 
##' @param x 
##' @param params 
##' @param nAnts 
##' @param nRuns 
##' @param instance 
##' @param outFile 
##' @param script 
##' @param n 
##' @return 
##' @author Alexander
pRunMe <- function(x,params,instance,script=COMMAND,n=20) {
    alpha <- params[x,"alpha"]
    beta <- params[x,"beta"]
    nRuns <- params[x,"nRuns"]
    nAnts <- params[x,"nAnts"]
    n <- params[x,"n"]
    res <- runMe(nAnts=nAnts,nRuns=nRuns,instance=instance,alpha=alpha,beta=beta,script=script,n=n)
    return(res)
}


library(parallel)


alpha <- seq(0.1,1,by=0.2)
beta <- seq(0.1,1,by=0.2)
nRuns <- c(5,10,20)
nAnts <- c(100,500,1000)
n <- 20
params <- expand.grid(alpha,beta,nRuns,nAnts,n)
colnames(params) <- c("alpha","beta","nRuns","nAnts","n")


optim_res <- mclapply(1:nrow(params),pRunMe,params=params,instance=SEL_INSTANCE,mc.cores=8)

## create object which will be saved
opt_res <- list(optim_res=optim_res,params=params)

optResFileString <- paste("output/aco_opt_",gsub("/","_",SEL_INSTANCE),".Rdata",sep="")
save(opt_res,file=optResFileString)



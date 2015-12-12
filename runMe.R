COMMAND <- "src/assignment4"


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
runMe <- function(nAnts=100,nRuns=10,instance="instances/automatic-1.txt",
                  outFile="TEST",alpha=1,beta=1,script=COMMAND,n=20) {
    res <- numeric(n)
    for(i in 1:n) {
        commandString <- paste(script,"--num-ants",nAnts,"--num-runs",nRuns,"--beta",beta,"--alpha",alpha,
                               "--input",instance,"--output",outFile)
        out <- system(commandString,intern=TRUE)
        crossingSum <- tail(out,1)
        ## regexp would be nice, but so far, stringsplit at " " and take the third element of the resulting vector
        res[i] <- as.numeric(strsplit(crossingSum," ")[[1]][3])
    }
    
    return(list(res=res,avg=mean(res),stdDevs=sd(res)))
}


pRunMe <- function(x,params,nAnts=100,nRuns=10,instance="instances/automatic-1.txt",
                   outFile="TEST",script=COMMAND,n=20) {
    alpha <- params[x,"alpha"]
    beta <- params[x,"beta"]
    res <- runMe(nAnts=nAnts,nRuns=nRuns,instance=instance,
                 outFile=outFile,alpha=alpha,beta=beta,script=script,n=n)
    return(res)
}


library(parallel)




alpha <- seq(0.1,1,by=0.1)
beta <- seq(0.1,1,by=0.1)
params <- expand.grid(alpha,beta)
colnames(params) <- c("alpha","beta")


test <- mclapply(1:nrow(params),pRunMe,params=params,instance="instances/automatic-4.txt",nAnts=10,mc.cores=8)


res <- matrix(NA,nrow=length(alpha),ncol=length(beta))




for(i in 1:length(alpha)) {
    cat(i," ")
    for(j in 1:length(beta)) {
        res[i,j] <- runMe(1000,20,"instances/automatic-4.txt","TEST",alpha=alpha[i],beta=beta[j])
    }
}


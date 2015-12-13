COMMAND <- "src/assignment4"
SEL_INSTANCE <- "instances/automatic-4.txt"



library(parallel)

source("runFuncs.R")


alpha <- c(0.1,1,2)
beta <- c(0.1,1,2)
nRuns <- c(5,10,20,50,100)
nAnts <- c(100,500,1000,2000)
n <- 10
params <- expand.grid(alpha,beta,nRuns,nAnts,n)
colnames(params) <- c("alpha","beta","nRuns","nAnts","n")


optim_res <- mclapply(1:nrow(params),pRunMe,params=params,instance=SEL_INSTANCE,mc.cores=8)

## create object which will be saved
opt_res <- list(optim_res=optim_res,params=params)

optResFileString <- paste("output/aco_opt_",gsub("/","_",SEL_INSTANCE),".Rdata",sep="")
save(opt_res,file=optResFileString)



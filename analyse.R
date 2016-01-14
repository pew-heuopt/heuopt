load("output/aco_opt_instances_automatic-4.txt.Rdata")


dat <- opt_res[[1]]
params <- opt_res[[2]]




generateComparisonMatrix <- function(d,p,cols2plot,subsetCols,subsetVals) {
    if(length(subsetCols)!=length(subsetVals))
        stop("error: subset cols and vals need to have same length")
    if(length(cols2plot)!=2)
        stop("error: right now, we can only have 2 columns")
    ## evaluate subset
    evalMat <- matrix(NA,ncol=length(subsetCols),nrow=nrow(p))
    colnames(evalMat) <- subsetCols
    for(i in 1:length(subsetCols)) {
        evalMat[,i] <- p[,subsetCols[i]]==subsetVals[i]
    }
    evalVec <- rowSums(evalMat)==length(subsetCols)
    if(sum(evalVec)==0)
        stop("result set is of size 0")

    ## select parameters and data
    d <- d[evalVec]
    p <- p[evalVec,]

    ## determine result matrix
    nRow <- length(unique(p[,cols2plot[1]]))
    nCol <- length(unique(p[,cols2plot[2]]))

    resMat <- matrix(NA,nrow=nRow,ncol=nCol)
    rownames(resMat) <- as.character(sort(unique(p[,cols2plot[1]])))
    colnames(resMat) <- as.character(sort(unique(p[,cols2plot[2]])))    

    ## now enter values

    for(i in 1:nrow(p)) {
        selRow <- as.character(p[i,cols2plot[1]])
        selCol <- as.character(p[i,cols2plot[2]])
        resMat[selRow,selCol] <- d[[i]]$avg
    }
    
    return(resMat)
}


library(xtable)

runTable <- generateComparisonMatrix(dat,params,c("nAnts","nRuns"),c("n","beta","alpha"),c(10,1,1))
runTableXtab <- xtable(runTable)

print(runTableXtab,file="src/report4/runTable.tex",floating=FALSE)



alphaBetaTab <- generateComparisonMatrix(dat,params,c("alpha","beta"),c("n","nAnts","nRuns"),c(10,2000,100))

alphaBetaXTab <- xtable(alphaBetaTab)
print(alphaBetaXTab,file="src/report4/alphabeta.tex",floating=FALSE)







undebug(generateComparisonMatrix)




load("output/res1.rdata")
load("output/res2.rdata")
load("output/res3.rdata")
load("output/res4.rdata")
load("output/res5.rdata")
load("output/res6.rdata")
load("output/res7.rdata")
load("output/res8.rdata")
load("output/res9.rdata")
load("output/res10.rdata")


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


analyseResult(res1)
analyseResult(res2)
analyseResult(res3)
analyseResult(res4)
analyseResult(res5)
analyseResult(res6)
analyseResult(res7)
analyseResult(res8)
analyseResult(res9)
analyseResult(res10)


debug(analyseResult)

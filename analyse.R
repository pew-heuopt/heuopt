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



generateComparisonMatrix(dat,params,c("nAnts","nRuns"),c("n","beta","alpha"),c(10,1,1))

generateComparisonMatrix(dat,params,c("alpha","beta"),c("n","nAnts","nRuns"),c(10,500,10))


undebug(generateComparisonMatrix)

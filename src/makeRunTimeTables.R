library(xtable)

### read in data
#objValues <- read.table("../output/crossings_stat.data",header=TRUE,sep=" ",row.names=1)


## read in results of the objective function
objValues <- read.table("../output/crossings_cnstr-random.data",header=TRUE,sep=" ",row.names=1)
runTimes <- read.table("../output/time_cnstr-random.data",header=TRUE,sep=" ",row.names=1)

res <- cbind(objValues[,c("min","avg","sd")],runTimes[,c("min", "avg","sd")])

resMat <- formatC(res[,1],digits=2,big.mark=",",format="f")
for(i in 2:ncol(res)) {
    resMat <- cbind(resMat,formatC(res[,i],digits=2,big.mark=",",format="f"))
}
## assign row and column names
rownames(resMat) <- rownames(res)
colnames(resMat) <- colnames(res)


### create table and write to file

mat <- xtable(resMat,align="l|rrr|rrr|")

sink("report/resultsTable.tex")

print(mat,hline.after=NULL,
      floating=FALSE,
      add.to.row=list(
           pos=list(-1,-1,0,nrow(res)),
           command=c('\\toprule ',
                    ' & \\multicolumn{2}{c}{Objective} & \\multicolumn{2}{c}{Runtime} \\\\',
                    '\\midrule ',
                    '\\bottomrule ')))

sink()

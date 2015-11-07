setwd("~/REPOS/heuopt/")
library(xtable)

crossings_num <- read.table("output/crossings_stat.data",sep=" ",header=TRUE,
                        row.names=1)

## change the order
crossings_num <- crossings_num[,c(4,8,7,5,9,2,6,3,1)]

stagnation_num <- read.table("output/stagnation_stat.data",sep=" ",header=TRUE,
                        row.names=1)

time_num <- read.table("output/time_stat.data",sep=" ",header=TRUE,
                        row.names=1)

### format the column headers
COLNAMES <- list()
COLNAMES[["lsr.1node.first"]] <- "\\begin{minipage}[c]{1.4cm}\\centering 1-Node \\\\ (first)\\end{minipage}"
COLNAMES[["lsr.1node.best"]] <- "\\begin{minipage}[c]{1.4cm}\\centering 1-Node \\\\ (best)\\end{minipage}"
COLNAMES[["lsr.1node.rand"]] <- "\\begin{minipage}[c]{1.4cm}\\centering 1-Node \\\\ (random)\\end{minipage}"
COLNAMES[["lsr.1edge.first"]] <- "\\begin{minipage}[c]{1.4cm}\\centering 1-Edge \\\\ (first)\\end{minipage}"
COLNAMES[["lsr.1edge.best"]] <- "\\begin{minipage}[c]{1.4cm}\\centering 1-Edge \\\\ (best)\\end{minipage}"
COLNAMES[["lsr.1edge.rand"]] <- "\\begin{minipage}[c]{1.4cm}\\centering 1-Edge \\\\ (random)\\end{minipage}"
COLNAMES[["lsr.1node.edge.first"]] <- "\\begin{minipage}[c]{1.4cm}\\centering 1-N-E \\\\ (first)\\end{minipage}"
COLNAMES[["lsr.1node.edge.best"]] <- "\\begin{minipage}[c]{1.4cm}\\centering 1-N-E \\\\ (best)\\end{minipage}"
COLNAMES[["lsr.1node.edge.rand"]] <- "\\begin{minipage}[c]{1.4cm}\\centering 1-N-E \\\\ (random)\\end{minipage}"



crossings <- toFormattedChar(crossings_num,digits=0,big.mark=",")
stagnation <- toFormattedChar(stagnation_num,digits=0,big.mark=",")
time <- toFormattedChar(time_num,digits=2,big.mark=",")
### merge them together..
for(selCol in colnames(crossings)) {
    for(selRow in rownames(crossings)) {
        crossings[selRow,selCol] <- paste("\\vspace{0.02cm} \\begin{minipage}[c]{1.5cm} \\centering",
                                          paste(crossings[selRow,selCol],"\\\\",
                                          stagnation[selRow,selCol]," / ",
                                          time[selRow,selCol],"",
                                          sep=""),"\\end{minipage}")
    }
}
colnames(crossings) <- as.character(COLNAMES[colnames(crossings)])
alignString <- paste("l",paste(rep("c",ncol(crossings)),sep="",collapse=""),sep="")
xtab <- xtable(crossings,align=alignString)
### print table and write it to a file
print(xtab,floating=FALSE,file="src/report2/resultsTab.tex",hline.after=NULL,
      sanitize.text.function=function(x){x},
      add.to.row=list(
           pos=list(-1,0,nrow(res),1,2,3,4,5,6,7,8,9),
           command=c('\\toprule ',
                    '\\midrule ',
                    '\\bottomrule ',' \\midrule ',' \\midrule ',' \\midrule ',
               ' \\midrule ',' \\midrule ',' \\midrule ',' \\midrule ',' \\midrule ',' \\midrule ')))










##' format matrix using formatC
##'
##' .. content for \details{} ..
##' @title 
##' @param x 
##' @param digits 
##' @param big.mark 
##' @param format 
##' @return 
##' @author Alexander
toFormattedChar <- function(x,digits=2,big.mark=".",format="f") {
    resMat <- formatC(x[,1],digits=digits,big.mark=big.mark,format=format)
    for(i in 2:ncol(x)) {
        resMat <- cbind(resMat,formatC(x[,i],digits=digits,big.mark=big.mark,format=format))
    }
    rownames(resMat) <- rownames(x)
    colnames(resMat) <- colnames(x)
    return(resMat)
}

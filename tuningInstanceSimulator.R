

vertices
pages


generateInstance <- function(nVert,nPages,nEdges) {
    maxEdges <- nVert*(nVert-1)/2
    if(nEdges>maxEdges)
        stop("too many edges")
    cat(nVert)
    cat("\n")
    cat(nPages)
    cat("\n")
    vertexOrder <- sample(1:nVert)-1
    for(i in 1:length(vertexOrder)) {
        cat(vertexOrder[i])
        cat("\n")
    }
    ## now let's generate some edges
    allEdges <- generateCompleteEdges(nVert)
    selEdgesInd <- sample(1:nrow(allEdges),nEdges)
    selEdges <- allEdges[selEdgesInd,]
    for(i in 1:nrow(selEdges)) {
        cat(selEdges[i,1]," ",selEdges[i,2]," [0] \n",sep="")
    }
    cat("\n")
}





generateCompleteEdges <- function(n) {
    edges <- t(apply(expand.grid(1:n-1,1:n-1),1,sort))
    edges <- edges[edges[,1]!=edges[,2],]
    edges <- unique(edges)
    return(edges)
}


### test simulator and check if we can run our optimization on this graph

# sink("instances/simulated/test1.txt")
# generateInstance(10,3,35) 
# sink()

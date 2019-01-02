const uuidv1 = require("uuid/v1")
const fs = require("fs")

let edges = []
let nodes = []
let nodeIDs = []
let mainNodeId = uuidv1()
nodes.push({
    id: mainNodeId,
    label: "MAIN"
})

const NUMBER_OF_LEVEL_1 = 200

for (let node = 0; node < NUMBER_OF_LEVEL_1; node++) {
    nodes.push({
        id: `node_${node}`,
        label: `node_${node}`
    })
    nodeIDs.push(`node_${node}`)
}

for (let node of nodes) {
    if (node.label !='MAIN') {
        edges.push({
            from: mainNodeId,
            to: node.id
        })
        let extraConnections = Math.floor(Math.random() * 10)
        if (extraConnections >= 2) {
            edges.push({
                from: node.id,
                to: nodeIDs[Math.floor(Math.random() * nodeIDs.length)]
            })
        }
        if (extraConnections >= 5) {
            edges.push({
                from: node.id,
                to: nodeIDs[Math.floor(Math.random() * nodeIDs.length)]
            })
        }

        if (extraConnections >= 7) {
            edges.push({
                from: node.id,
                to: nodeIDs[Math.floor(Math.random() * nodeIDs.length)]
            })
        }
        if (extraConnections >= 9) {
            edges.push({
                from: node.id,
                to: nodeIDs[Math.floor(Math.random() * nodeIDs.length)]
            })
        }
    }
}

fs.writeFileSync("edges.js", "let edgesArray = ".concat(JSON.stringify(edges)))
fs.writeFileSync("nodes.js", "let nodesArray = ".concat(JSON.stringify(nodes)))

import luaDocs;
import re;
import xml.etree.ElementTree as ET
import string;

class HtmlOutput:
    
    header = """
        <head>
            <title>$title</title>
            <link rel='stylesheet' type='text/css' href='../css/styles.css'/>
        </head>
    """;
    class_body = """
        <div class='class_body'></div>
    """;
    class_title = """
        <h2>$title</h2>
    """;
    comment = """
        <div class='comment'>$comment</div>
    """;
    table_contents = """
        <div class='table_of_contents rounded'></div>
    """;
    functions = """
        <div class='functions'></div>
    """;
    function_instance = """
        <div class='function_instance rounded'></div>
    """;
    function_name = """
        <div>
            <code>$static</code><span id='$id' class='name'>$name</span>
        </div>
    """;
    function = """
        <div class='function'></div>
    """;
    parameters = """
        <div class='parameters'>Parameters:</div>
    """;
    type_list = """
        <div class='type_list'></div>
    """
    param_short = """
        <span class='param_short'><span class='type'>$type</span> <span class='name'>$name</span><span class='optional'>$optional</span></span>
    """;
    param_full = """
        <div class='param_full'>
            <span class='type'>$type</span> <span class='name'>$name</span><span class='optional'>$optional</span><span>: </span><span class='comment'>$comment</span>
        </div>
    """;
    returns = """
        <div class='returns'>Returns:</div>
    """
    returns_short = """
        <span class='returns_short'><span class='type'>$type</span></span>
    """;
    returns_full = """
        <div class='returns_full'>
            <span class='type'>$type</span><span>: </span><span class='comment'>$comment</span>
        </div>
    """;

    def __init__(self):
        self.indent = 0;
        self.html = ET.ElementTree(ET.Element("html"));

    def toTitle(self, str):
        return str.replace("_", " ").title();
        
    def load(self, classDoc):
        self.classDoc = classDoc;
        root = self.html.getroot();
        node = self.createNode(HtmlOutput.header, title=classDoc.name);
        root.append(node);
        self.body = self.createNode("<body></body>");
        root.append(self.body);
        
        self.classBody = self.createNode(HtmlOutput.class_body);
        self.body.append(self.classBody);
        
        self.classBody.append(self.createNode(HtmlOutput.class_title, title=self.toTitle(classDoc.name)));
        self.classBody.append(self.createNode(HtmlOutput.comment, comment=classDoc.classComment));
        self.classBody.append(self.createTableOfContents());
        
        self.functions = self.createFunctions();
        self.classBody.append(self.functions);
    
    def createIndex(self, classes, header, headerComment):
        root = self.html.getroot();
        node = self.createNode(HtmlOutput.header, title=header);
        root.append(node);
        self.body = self.createNode("<body></body>");
        root.append(self.body);
        
        self.classBody = self.createNode(HtmlOutput.class_body);
        self.body.append(self.classBody);
        
        self.classBody.append(self.createNode(HtmlOutput.class_title, title=header));
        self.classBody.append(self.createNode(HtmlOutput.comment, comment=headerComment));

        table = self.createNode(HtmlOutput.table_contents);
        self.classBody.append(table);

        list = self.createNode("<ul></ul>");
        table.append(list);

        for classDocName, classDoc in classes.items():
            list.append(self.createNode("<li><a href='$link'>$text</a></li>", link=classDocName + ".html", text=self.toTitle(classDocName)));
        
    def write(self, outputPath):
        self.output = open(outputPath, "w+");
        self.writeNode(self.html.getroot());
        
    def writeNode(self, node):
        whitespace = "\t" * self.indent;
        self.indent += 1;
        
        if len(node.attrib) == 0:
            self.output.write(whitespace + "<" + node.tag + ">");
        else:
            self.output.write(whitespace + "<" + node.tag);
            for key,value in node.attrib.items():
                if len(value) > 0:
                    self.output.write(" " + key + "='" + value + "'");
                # We'll assume that most attributes that are empty
                # are potentially valid except for 'id'.
                elif key != "id":
                    self.output.write(" " + key);
            self.output.write(">");
        childList = list(node);
        
        if node.text:
            nodeText = str(node.text).rstrip();
            if node.tag == "pre":
                nodeText = nodeText.replace("\n ", "\n");
            
            if len(nodeText) > 0:
                if len(childList) == 0:
                    self.output.write(nodeText);
                else:
                    whitespace2 = whitespace + "\t";
                    self.output.write("\n" + whitespace2 + nodeText);
        
        if len(childList) > 0:
            self.output.write("\n");
            
        for child in childList:
            self.writeNode(child);
            
        if len(childList) == 0:
            self.output.write("</" + node.tag + ">\n");
        else:
            self.output.write(whitespace + "</" + node.tag + ">\n");
        self.indent -= 1;
    
    def createNode(self, template, **args):
        s = string.Template(template).substitute(args);
        return ET.fromstring(s);
        
    def createTableOfContents(self):
        node = self.createNode(HtmlOutput.table_contents);
        def outputFuncs(funcs):            
            for funcDescName, funcDesc in funcs.items():
                for funcInst in funcDesc.instances:
                    id = funcDesc.name + "_" + funcInst.idList();
                    link = self.createNode("<a href='#$link'></a>", link=id);
                    name = self.createMainFuncName(funcDesc.name, funcInst, False);
                    link.append(name);
                    node.append(link);
                    
        outputFuncs(self.classDoc.specialFuncs);
        outputFuncs(self.classDoc.funcs);
                
        return node;
        
    def createFunctions(self):
        functions = self.createNode(HtmlOutput.functions);
        
        def outputFuncs(funcs):
            for funcDescName, funcDesc in funcs.items():
                try:
                    if funcDesc.cppName == "NULL":
                        continue;
                    instDiv = self.createNode(HtmlOutput.function_instance);
                    functions.append(instDiv);
                    for funcInst in funcDesc.instances:
                        func = self.createNode(HtmlOutput.function);
                        func.append(self.createMainFuncName(funcDesc.name, funcInst));
                        func.append(self.createNode(HtmlOutput.comment, comment=funcInst.comment));
                        if len(funcInst.params) > 0:
                            func.append(self.createNode(HtmlOutput.parameters));
                            params = self.createNode(HtmlOutput.type_list);
                            func.append(params);
                            for param in funcInst.params:
                                optional = "= " + param.optional if param.optional else "";
                                params.append(self.createNode(HtmlOutput.param_full, type=param.type, name=param.name, comment=param.comment, optional=optional));
                                
                        if len(funcInst.returns) > 0:
                            func.append(self.createNode(HtmlOutput.returns));
                            returns = self.createNode(HtmlOutput.type_list);
                            func.append(returns);
                            for ret in funcInst.returns:
                                returns.append(self.createNode(HtmlOutput.returns_full, type=ret.type, comment=ret.comment));
                                
                        instDiv.append(func);
                except (ET.ParseError):
                    print("Error parseing HTML for function:", self.classDoc.name + "." + funcDescName);
                    
        outputFuncs(self.classDoc.specialFuncs);
        outputFuncs(self.classDoc.funcs);
                
        return functions;
        
    def createMainFuncName(self, name, instance, includeId = True):
        id = "";
        if includeId:
            id = name + "_" + instance.idList();
        
        static = "";
        if instance.isStatic:
            static = "static ";
        node = self.createNode(HtmlOutput.function_name, name=name, id=id, static=static);
        node.append(self.createNode("<span>(</span>"));
        first = True;
        for param in instance.params:
            if first:
                first = False;
            else:
                node.append(self.createNode("<code>,</code>"));
            optional = "= " + param.optional if param.optional else "";
            node.append(self.createNode(HtmlOutput.param_short, type=param.type, name=param.name, optional=optional));
        node.append(self.createNode("<span>)</span>"));
        
        if name == "new" or name == "__gc":
            return node;
            
        node.append(self.createNode("<code>-></code>"));
        
        node.append(self.createNode("<span>(</span>"));
        first = True;
        for ret in instance.returns:
            if first:
                first = False;
            else:
                node.append(self.createNode("<code>,</code>"));
            node.append(self.createNode(HtmlOutput.returns_short, type=ret.type));
        node.append(self.createNode("<span>)</span>"));
        
        return node;

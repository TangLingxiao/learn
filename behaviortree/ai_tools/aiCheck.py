#coding : utf-8
import os
import sys
try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET

sys.path.append("base/")

rule = "base/ai_rules.xml"

ai_path = r"../ConfigXlsx/aiconfig/"

def Add2File( file_ , str_ ):
	f = open(file_, 'w')
	f.writelines(str_)
	f.close()

class Errors():
    def __init__(self, nodetype, subtype, nodename, reason):
        self.type = nodetype
        self.subtype = subtype
        self.name = nodename
        self.reason = reason

    def out(self):
        return (str(self.type), str(self.subtype), str(self.name), str(self.reason))

class CheckAi():
    def __init__(self):
        self.node_type = set()#node type
        self.node_type_has_type = {}#node has type
        self.node_param_with_type = {}#node params with type
        self.node_param_without_type = {}
        self.errors = {}
        self.loadRule()

    def loadRule(self):
        tree = ET.parse(rule)
        root = tree.getroot()
        for one in root:
            self.node_type.add(one.tag)
            if one.get('type') is not None:
                if self.node_type_has_type.has_key(one.tag) is False:
                    self.node_type_has_type[one.tag] = set()
                self.node_type_has_type[one.tag].add(one.get('type'))
                for k,v in one.attrib.items():
                    if self.node_param_with_type.has_key(one.get('type')) is False:
                        self.node_param_with_type[one.get('type')] = set()
                    self.node_param_with_type[one.get('type')].add(k)
            else:
                if self.node_param_without_type.has_key(one.tag) is False:
                    self.node_param_without_type[one.tag] = set()
                for k, v in one.attrib.items():
                    self.node_param_without_type[one.tag].add(k)
        
    
    #check spell error or param error
    #return errorcode, nodetype, nodename, errorreason
    def checkNodeTypeAndParam(self, root, filename):
        for one in root.getchildren():
            if one.tag  not in self.node_type:
                self.errors[filename].append(Errors(one.tag, one.get('type'), one.get('name'), one.tag))
            if self.node_type_has_type.has_key(one.tag) and one.get('type')  not in self.node_type_has_type[one.tag]:
                self.errors[filename].append(Errors(one.tag, one.get('type'),one.get('name'), 'type error '+str(one.get('type'))))
            if one.get('type') is not None:
                for k, v in one.attrib.items():
                    if self.node_param_with_type.has_key(one.get('type')) and k  not in self.node_param_with_type[one.get('type')]:
                        self.errors[filename].append(Errors(one.tag, one.get('type'),one.get('name'), 'dont have param '+k))
            if one.get('type') is None:
                for k, v in one.attrib.items():
                    if self.node_param_without_type.has_key(one.tag) and k  not in self.node_param_without_type[one.tag]:
                        self.errors[filename].append(Errors(one.tag, one.get('type'),one.get('name'), 'dont have param '+k))
            self.checkNodeTypeAndParam(one,filename)
            
    #check action node 
    #only one action node is allowed in one level(except change action)
    def checkUniqueAction(self, root, filename):
        children = root.getchildren()
        if len(children) == 0:
            return
        actions = 0
        for child in children:
            if child.tag == 'action' and child.get('type') != 'change':# and child.get('type') != 'exec' and child.get('type') != 'safe':
                actions += 1
        if actions > 1:
            self.errors[filename].append(Errors(root.tag, root.get('type'),root.get('name'), 'too many actions'))
        for child in children:
            self.checkUniqueAction(child, filename)

    #check safe
    #safe action is just below root node
    def checkSafeAction(self, root, filename):
        safes = 0
        find = False
        for one in root:
            if one.get('type') == 'safe':
                find = True
        if find is False:
            self.errors[filename].append(Errors(root.tag, root.get('type'),root.get('name'), 'root node has no safe node'))
        for one in root.iter('action'):
            if one.get('type') == 'safe':
                safes += 1
        if safes == 0:
            self.errors[filename].append(Errors(root.tag, root.get('type'),root.get('name'), 'there is no safe node'))
        if safes > 1:
            self.errors[filename].append(Errors(root.tag, root.get('type'),root.get('name'), 'too many safe node'))
        
    #check exec
    #exec action must have at least one
    def checkExecAction(self, root, filename):
        i = 0
        for one in root.iter('action'):
            if one.get('type') == 'exec':
                i += 1
        if i == 0:
            self.errors[filename].append(Errors(root.tag, root.get('type'),root.get('name'),'there is no exec node'))
        if i > 1:
            self.errors[filename].append(Errors(root.tag, root.get('type'),root.get('name'),'too many exec node'))

    #check target
    #after action node must have select target condition node
    def checkTargetCondition(self, root, filename):
        for child in root.getchildren():
            if child.tag == 'action' and child.get('type') != 'safe' and child.get('type') != 'exec':
                find_target = False
                for cond in root.iter('condition'):
                    if cond.get('type') is not None:
                        if cond.get('type').find('select') != -1:
                            find_target = True
                            break
                if find_target is False:
                    self.errors[filename].append(Errors(root.tag,root.get('type'),root.get('name'),'no select target after action'))
            self.checkTargetCondition(child, filename)

    def Run(self):
        ls = os.listdir(ai_path)
        for i in range(0, len(ls)):
            file_ = os.path.join(ai_path, ls[i])
            if os.path.isfile(file_):
                if self.errors.has_key(file_) is False:
                    self.errors[file_] = []
                print("%s begin........." %(file_,))
                tree = ET.parse(file_)
                root = tree.getroot()
                self.checkNodeTypeAndParam(root,file_)
                self.checkUniqueAction(root,file_)
                self.checkSafeAction(root,file_)
                self.checkExecAction(root,file_)
                self.checkTargetCondition(root,file_)
                print("%s end........." %(file_,))
                
        f = open('error.txt', 'w')
        for k,v in self.errors.items():
            if len(v) > 0:
                f.write('\n' + str(k) + ' error nums: %d' %(len(v),) + '\n')
                for one in v:
                    str_ = "error, node type:%s, node subtype: %s, node name:%s, error desc:%s \n"%(one.out())
                    f.write(str_)
        f.close()



if __name__ == '__main__':
    check = CheckAi()
    check.Run()
    os.system('pause')
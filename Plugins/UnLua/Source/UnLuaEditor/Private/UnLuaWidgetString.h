//TEXTBLOCK
const char* LUA_TEXTBLOCK_STR_TEMPLATE =
R"(	-- self.WIDGET_NAME:SetText("Your String"))";

//IMAGE
const char* LUA_IMAGE_STR_TEMPLATE =
R"(	-- UIHelper.SetImageRes(self.WIDGET_NAME, resPath, bUseResSize))";

//BUTTON
const char* LUA_BUTTON_STR_TEMPLATE =
R"(	self:AddTapListener(self.WIDGET_NAME, self.OnWIDGET_NAMEClicked))";

const char* LUA_BUTTON_FUNCTION_TEMPLATE =
R"(function CLASS_NAME:OnWIDGET_NAMEClicked()
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMEClicked() ****" )
end)";

//PROGRESSBAR
const char* LUA_PROGRESSBAR_STR_TEMPLATE =
R"(	-- self.WIDGET_NAME:SetPercent(0.5))";

//RICHTEXTBLOCK
const char* LUA_RICHTEXTBLOCK_STR_TEMPLATE =
R"(	-- self.WIDGET_NAME:SetText("<red>Your String</>"))";

//SLIDER
const char* LUA_SLIDER_STR_TEMPLATE =
R"(	self.WIDGET_NAME.OnValueChanged:Add(self, self.OnWIDGET_NAMEValueChanged))";

const char* LUA_SLIDER_FUNCTION_TEMPLATE =
R"(function CLASS_NAME:OnWIDGET_NAMEValueChanged( Value )
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMEValueChanged()  Value = " .. tostring(Value) .. " ****" )
end)";

//CHECKBOX
const char* LUA_CHECKBOX_STR_TEMPLATE =
R"(	self.WIDGET_NAME.OnCheckStateChanged:Add(self, self.OnWIDGET_NAMEStateChanged))";

const char* LUA_CHECKBOX_FUNCTION_TEMPLATE =
R"(function CLASS_NAME:OnWIDGET_NAMEStateChanged( bChecked )
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMEStateChanged()  bChecked = " .. tostring(bChecked) .. " ****" )
end)";


//LISTVIEW、TILEVIEW、TREEVIEW
const char* LUA_LISTVIEW_STR_TEMPLATE =
R"(	-- self.WIDGET_NAMELogic = ListViewLogic.new(self, self.WIDGET_NAME, listData))";

//COMBOBOXSTRING
const char* LUA_COMBOBOXSTRING_STR_TEMPLATE =
R"(	self.WIDGET_NAME:AddOption("str1")
	self.WIDGET_NAME:AddOption("str2")
	self.WIDGET_NAME:SetSelectedIndex(0)
	self.WIDGET_NAME.OnSelectionChanged:Add(self, self.OnWIDGET_NAMESelectionChanged))";

const char* LUA_COMBOBOXSTRING_FUNCTION_TEMPLATE =
R"(function CLASS_NAME:OnWIDGET_NAMESelectionChanged( SelectedItem, SelectionType )
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMESelectionChanged()  SelectedItem = " .. tostring(SelectedItem)  .. " ****" )
end)";

//EDITABLETEXT、EDITABLETEXTBOX、MULTILINEEDITABLETEXT、MULTILINEEDITABLETEXTBOX
const char* LUA_EDITABLETEXT_STR_TEMPLATE =
R"(	self.WIDGET_NAME:SetText("Your Text String")
	self.WIDGET_NAME:SetHintText("Your HintText String")
	self.WIDGET_NAME.OnTextChanged:Add(self, self.OnWIDGET_NAMETextChanged)
	self.WIDGET_NAME.OnTextCommitted:Add(self, self.OnWIDGET_NAMETextCommitted))";

const char* LUA_EDITABLETEXT_FUNCTION_TEMPLATE =
R"(function CLASS_NAME:OnWIDGET_NAMETextChanged( Text )
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMETextChanged()  Text = " .. tostring(Text)  .. " ****" )
end

function CLASS_NAME:OnWIDGET_NAMETextCommitted( Text, Type )
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMETextCommitted()  Text = " .. tostring(Text) .. "   Type = " .. tostring(Type)  .. " ****" )
end)";

//SPINBOX
const char* LUA_SPINBOX_STR_TEMPLATE =
R"(	self.WIDGET_NAME:SetValue(1)
	self.WIDGET_NAME.OnValueChanged:Add(self, self.OnWIDGET_NAMEValueChanged)
	self.WIDGET_NAME.OnValueCommitted:Add(self, self.OnWIDGET_NAMEValueCommitted)
	self.WIDGET_NAME.OnBeginSliderMovement:Add(self, self.OnWIDGET_NAMEBeginSliderMovement)
	self.WIDGET_NAME.OnEndSliderMovement:Add(self, self.OnWIDGET_NAMEEndSliderMovement))";

const char* LUA_SPINBOX_FUNCTION_TEMPLATE =
R"(function CLASS_NAME:OnWIDGET_NAMEValueChanged( Text )
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMEValueChanged()  Text = " .. tostring(Text)  .. " ****" )
end

function CLASS_NAME:OnWIDGET_NAMEValueCommitted( Text, Type )
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMEValueCommitted()  Text = " .. tostring(Text) .. "   Type = " .. tostring(Type)  .. " ****" )
end

function CLASS_NAME:OnWIDGET_NAMEBeginSliderMovement()
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMEBeginSliderMovement() ****" )
end

function CLASS_NAME:OnWIDGET_NAMEEndSliderMovement()
	Log.i( "**** CLASS_NAME:OnWIDGET_NAMEEndSliderMovement() ****" )
end)";



//UNKNOWN TYPE
const char* LUA_UNKNOWN_STR_TEMPLATE =
R"(	-- self.WIDGET_NAME)";


//ANIMATION
const char* LUA_ANIMATION_STR_TEMPLATE =
R"(	-- self:PlayAnimation(self.ANIMATION_NAME)
	-- self:BindToAnimationFinished(self.ANIMATION_NAME, {self, self.OnANIMATION_NAMEFinished}))";

const char* LUA_ANIMATION_FUNCTION_TEMPLATE =
R"(-- function CLASS_NAME:OnANIMATION_NAMEFinished()
-- end)";



//EventGraphs
const char* LUA_EVENTGRAPHS_FUNCTION_TEMPLATE =
R"(-- function CLASS_NAME:EVENTGRAPHS( PARAMS )
-- end)";


//LUA COMMON
const char* LUA_DEFAULT_STR_TEMPLATE =
R"(	-- self:AddEventListener(EEventType.XXX, self.OnEventXXX))";

const char* LUA_DEFAULT_FUNCTION_TEMPLATE =
R"(-- function CLASS_NAME:OnEventXXX( EventType, Data )
-- end)";
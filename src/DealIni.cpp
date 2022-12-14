//
// Created by  shecannotsee on 2023/1/7.
//

#include "DealIni.h"

sheIni::DealIni::DealIni()
    : result_(INI_line_state::defaultValue),
      section_(std::string()),
      key_(std::string()),
      value_(std::string()),
      type_(sheIni::INI_value_type::defaultValue){
};

sheIni::FSM_state sheIni::DealIni::interface(char ch) {
  switch (ch) {
    case static_cast<int>(INI_char_state::notes_numberSign) : {
      return this->singleNote(ch);
      break;
    }
    case static_cast<int>(INI_char_state::section_start) : {
      return this->multipleSection(ch);
      break;
    }
    case static_cast<int>(INI_char_state::section_end) : {
      return this->multipleSection(ch);
      break;
    }
    case static_cast<int>(INI_char_state::type_start) : {
      return this->multipleType(ch);
      break;
    }
    case static_cast<int>(INI_char_state::type_end) : {
      return this->multipleType(ch);
      break;
    }
    case static_cast<int>(INI_char_state::equal) : {
      return this->singleEqual(ch);
      break;
    }
    case static_cast<int>(INI_char_state::lineEnd) : {
      return this->singleLineEnd(ch);
      break;
    }
    case static_cast<int>(INI_char_state::WindowsLineBreak_first) : {
      return this->multipleWindowsLineBreak(ch);
      break;
    }
    case static_cast<int>(INI_char_state::linuxLineBreak) : {
      return this->singleLinuxLineBreak(ch);
      break;
    }
    default : {
      this->dealOtherChar(ch);
      break;
    }

  }
}

sheIni::FSM_state sheIni::DealIni::singleNote(char ch) {
  result_ = INI_line_state::note;
  return sheIni::FSM_state::Sizzle;
};

sheIni::FSM_state sheIni::DealIni::multipleSection(char ch) {
  if (ch == (int)INI_char_state::section_start) {
    result_ = INI_line_state::section;
  } else if (ch==(int)INI_char_state::section_end) {
    result_ = INI_line_state::defaultValue;
  } else {
    result_ = INI_line_state::error;
  }
  return sheIni::FSM_state::Sizzle;
};

sheIni::FSM_state sheIni::DealIni::multipleType(char ch) {
  if (ch == (int)INI_char_state::type_start) {
    result_ = INI_line_state::TypeValue;
  } else if (ch==(int)INI_char_state::type_end) /*?????????type??????????????????kv????????????*/ {
    result_ = INI_line_state::defaultValue;
  } else {
    result_ = INI_line_state::error;
  }
  return sheIni::FSM_state::Sizzle;
}

sheIni::FSM_state sheIni::DealIni::singleEqual(char ch) {
  result_ = INI_line_state::noTypeValue;
  return sheIni::FSM_state::Sizzle;
};

sheIni::FSM_state sheIni::DealIni::singleLineEnd(char ch) {
  // result_ ???????????????end??????
//  result_ = INI_line_state::end;
  return sheIni::FSM_state::Sizzle;
};

sheIni::FSM_state sheIni::DealIni::singleLinuxLineBreak(char ch) {
  // result_ ???????????????end??????
  if (result_== INI_line_state::mayBeEnd) /*??????windows???????????????*/ {
//    result_ = INI_line_state::end;
  } else /*??????,???????????????????????????????????????*/ {
//    result_ = INI_line_state::end;
  }
  return sheIni::FSM_state::Stop;
};

sheIni::FSM_state sheIni::DealIni::multipleWindowsLineBreak(char ch) {
  // result_ ???????????????end??????
//  result_ = INI_line_state::mayBeEnd;
  return sheIni::FSM_state::Stop;
};

sheIni::FSM_state sheIni::DealIni::dealOtherChar(char ch) {
  if (ch == (int )INI_char_state::space) {
    // Space
  }
  else if (result_==INI_line_state::note) /*??????????????????,??????????????????????????????????????????*/ {
    key_ += ch;
  }
  else if (result_==INI_line_state::section) {
    section_ += ch;
  }
  else if (result_==INI_line_state::defaultValue) /*????????????????????????????????????*/ {
    key_ += ch;
  }
  else if (result_==INI_line_state::noTypeValue) /*value??????*/ {
    value_ += ch;
  }
  else if (result_==INI_line_state::TypeValue) /*????????????????????????*/ {
    if (ch == (int)INI_value_type::filePath) {
      type_ = INI_value_type::filePath;
    } else if (ch == (int)INI_value_type::FPN) {
      type_ = INI_value_type::FPN;
    } else if (ch == (int)INI_value_type::integer) {
      type_ = INI_value_type::integer;
    } else {
      type_ = INI_value_type::error;
    }
  }
  else if (result_==INI_line_state::end) {
    return sheIni::FSM_state::ERROR;
  }

  return sheIni::FSM_state::Sizzle;
};

void sheIni::DealIni::setDefault() {
  result_ = INI_line_state::defaultValue;
  section_.clear();
  key_.clear();
  value_.clear();
  type_ = sheIni::INI_value_type::defaultValue;
};

std::tuple<sheIni::INI_line_state, sheIni::INI_value_type, std::string, std::string> sheIni::DealIni::get() {
  return std::make_tuple(result_,type_,key_,value_);
}


#include "ppstr_parser.h"
#include <cstring>
#include <functional>

#include <cctype>

template<typename T>
int parse_ppstr(const char *ppstr, T raw, double *res)
{
    const char *ps = ppstr;
    char local_buf[1024];

    double in = static_cast<double>(raw);

    while(*ps != '\0') {
        char *pbuf = local_buf;
        memset(local_buf, 0, sizeof(local_buf));
        while(*ps == '-' || *ps=='.' || isdigit(*ps)) {
            *pbuf = *ps;
            pbuf ++;
            ps ++;
        }
        double arg = atof(local_buf);
        switch(*ps){
            case('a'):
                in = in + arg;
            break;
            case('s'):
                in = in - arg;
            break;
            case('m'):
                in = in * arg;
            break;
            case('d'):
                in = in / arg;
            break;
            default:
                 return -1;
            break;
        }
        ps ++;
    }
    (*res) = in;
    return 0;
}

int post_process_cvt_by_str(CONVERTER *cvt, char *PP_STR, char type){
    double result;
    switch(type){
    case('d'):
    case('D'):  
        //signed int
        // TODO: This Do have some problem
        parse_ppstr(PP_STR, cvt->i32, &result);
        cvt->i32 = static_cast<int>(result);
    break;
    case('u'):
    case('U'):  
        //Unsigned int
        parse_ppstr(PP_STR, cvt->u32, &result);
        cvt->u32 = static_cast<uint32_t>(result);
    break;
    case('f'):
    case('F'):  
        //Float
        parse_ppstr(PP_STR, cvt->sg, &result);
        cvt->sg = result;
    break;
    case('g'):
    case('G'):
        //Double
        parse_ppstr(PP_STR, cvt->sg, &result);
        cvt->sg = result;
    break;
    case('N'):
    case('n'):
        //Ignore
    break;
    default:
        return -5;
    }
    return 0;
}
